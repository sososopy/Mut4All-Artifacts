use crate::mutator::Mutator;
use syn::parse_quote;

pub struct Nested_Impl_Trait_In_Closure_Param_42;

impl Mutator for Nested_Impl_Trait_In_Closure_Param_42 {
    fn name(&self) -> &str {
        "Nested_Impl_Trait_In_Closure_Param_42"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(item_fn) = item {
                if let syn::ReturnType::Type(_, return_type) = &mut item_fn.sig.output {
                    if let syn::Type::ImplTrait(type_impl_trait) = &mut **return_type {
                        for bound in &mut type_impl_trait.bounds {
                            if let syn::TypeParamBound::Trait(trait_bound) = bound {
                                let path = &mut trait_bound.path;
                                if let Some(segment) = path.segments.iter_mut().find(|seg| seg.ident == "Fn") {
                                    if let syn::PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                        if let Some(arg) = args.args.iter_mut().next() {
                                            if let syn::GenericArgument::Type(ty) = arg {
                                                *ty = syn::Type::ImplTrait(syn::TypeImplTrait {
                                                    impl_token: syn::token::Impl::default(),
                                                    bounds: {
                                                        let mut bounds = syn::punctuated::Punctuated::new();
                                                        bounds.push(syn::TypeParamBound::Trait(syn::TraitBound {
                                                            paren_token: None,
                                                            modifier: syn::TraitBoundModifier::None,
                                                            lifetimes: None,
                                                            path: parse_quote!(T),
                                                        }));
                                                        bounds
                                                    },
                                                });
                                            }
                                        }
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
        ""
    }
}