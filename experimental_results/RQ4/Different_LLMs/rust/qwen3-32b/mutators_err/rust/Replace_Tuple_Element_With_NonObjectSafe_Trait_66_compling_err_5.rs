use crate::mutator::Mutator;
use syn::punctuated::Punctuated;
use syn::token::Dyn;

pub struct Replace_Tuple_Element_With_NonObjectSafe_Trait_66;

impl Mutator for Replace_Tuple_Element_With_NonObjectSafe_Trait_66 {
    fn name(&self) -> &str {
        "Replace_Tuple_Element_With_NonObjectSafe_Trait_66"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(s) = item {
                for field in &mut s.fields {
                    if let syn::Type::Tuple(tuple) = &mut field.ty {
                        for i in 0..tuple.elems.len() - 1 {
                            if let syn::Type::Path(path) = &tuple.elems[i] {
                                let mut bounds = Punctuated::new();
                                bounds.push(syn::TypeParamBound::Trait(syn::TraitBound {
                                    lifetimes: syn::BoundLifetimes::default(),
                                    path: path.path.clone(),
                                    modifier: syn::TraitBoundModifier::None,
                                    paren_token: None,
                                }));
                                let new_type = syn::Type::TraitObject(syn::TypeTraitObject {
                                    dyn_token: Some(Dyn::default()),
                                    bounds,
                                });
                                tuple.elems[i] = new_type;
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}