use crate::mutator::Mutator;
use syn::File;

struct Replace_Const_Value;

impl Mutator for Replace_Const_Value {
    fn name(&self) -> &str {
        "Replace_Const_Value"
    }

    fn chain_of_thought(&self) -> &str {
        "Replace constant values in the AST"
    }

    fn mutate(&self, file_ast: &mut File) {
        // implement mutation logic here
    }
}