#include"zpacker.h"
#include"packer_tools.h"

bool filepakcer(size_t original_file_size){
    struct entry	original_entry;
    size_t		shift_amount; 

    if (!find_entry(&original_entry, original_safe)
	|| !define_shift_amount(&original_entry, &shift_amount)
	|| !copy_to_clone(original_entry.end_of_last_section, shift_amount, original_file_size)
	|| !adjust_references(shift_amount, &original_entry)
	|| !adjust_sizes(shift_amount)
	|| !setup_load_code(&original_entry)
	|| !change_entry(&original_entry)){
        return false;
    }
    
    return true;
}