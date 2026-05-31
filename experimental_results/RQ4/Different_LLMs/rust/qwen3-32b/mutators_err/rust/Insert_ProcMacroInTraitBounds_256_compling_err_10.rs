use syn::parse_quote;
use crate::mutator::Mutator;

pub struct Insert_ProcMacroInTraitBounds_256;

impl Mutator for Insert_ProcMacroInTraitBounds_256 {
    fn name(&self) -> &str {
        "Insert_ProcMacroInTraitBounds_256"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Type(item_type) = item {
                if let syn::Type::ImplTrait(impl_trait) = item_type.ty.as_mut().unwrap() {
                    if let Some(last_bound) = impl_trait.bounds.last_mut() {
                        *last_bound = parse_quote!(macro_that_expands_to_trait!());
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets type aliases with compound trait bounds and replaces the last trait bound with a macro invocation that expands to a trait. This introduces macro-generated paths in trait bound positions, testing the resolver's ability to handle nested macro expansions and potentially triggering errors in unwrap logic when macro expansions are mishandled."
    }
}