use syn::parse_quote;
use syn::Item;
use syn::TypeParamBound;
use syn::PathArguments;
use syn::GenericArgument;

pub struct Trait_Bound_Modification_110;

impl Mutator for Trait_Bound_Modification_110 {
    fn name(&self) -> &str {
        "Trait_Bound_Modification_110"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for supertrait in &mut trait_item.supertraits {
                    if let TypeParamBound::Trait(trait_bound) = supertrait {
                        if let Some(path_segment) = trait_bound.path.segments.last_mut() {
                            if let PathArguments::AngleBracketed(args) = &mut path_segment.arguments {
                                for arg in &mut args.args {
                                    if let GenericArgument::Type(ty) = arg {
                                        *ty = parse_quote!(i32);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets trait bounds in trait definitions or implementations, particularly those involving associated types and constraints. It identifies a trait definition or implementation that involves associated types and trait bounds and modifies the associated type constraint by introducing an additional bound or removing an existing one. This can involve altering the constraints on associated types or altering the trait hierarchy."
    }
}