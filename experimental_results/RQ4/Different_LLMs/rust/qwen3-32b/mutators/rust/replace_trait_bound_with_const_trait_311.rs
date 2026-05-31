use syn::parse_quote;
use crate::mutator::Mutator;
use syn::FnArg;
use syn::Type;
use syn::TypeParamBound;
use syn::TraitBoundModifier;

pub struct Replace_Trait_Bound_With_Const_Trait_311;

impl Mutator for Replace_Trait_Bound_With_Const_Trait_311 {
    fn name(&self) -> &str {
        "Replace_Trait_Bound_With_Const_Trait_311"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.constness.is_some() {
                    continue;
                }
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::TraitObject(trait_object) = &mut *pat_type.ty {
                            trait_object.dyn_token = None;
                            for bound in &mut trait_object.bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    trait_bound.modifier = TraitBoundModifier::Maybe(parse_quote!(?));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces valid trait bounds in function parameters with `~const Trait`, omitting `dyn` if present. This creates invalid trait bounds that bypass standard syntax rules, testing the compiler's handling of mixed const/lifetime parameters and potentially exposing assertion failures in trait bound analysis."
    }
}