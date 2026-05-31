use syn::{Item, ReturnType, Type, token, Ident, PathArguments, AngleBracketedGenericArguments, GenericArgument, TypePath, PathSegment, punctuated, TypeParamBound, TraitBound, TraitBoundModifier, TypeTraitObject};
use proc_macro2::Span;
use crate::mutator::Mutator;

struct Replace_Closure_Return_Type_With_Interface_Type_42;

impl Mutator for Replace_Closure_Return_Type_With_Interface_Type_42 {
    fn name(&self) -> &str {
        "Replace_Closure_Return_Type_With_Interface_Type_42"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        let dyn_token = token::Dyn { span: Span::call_site() };
                        let path = syn::Path {
                            leading_colon: None,
                            segments: punctuated::Punctuated::from_iter(vec![PathSegment {
                                ident: Ident::new("Fn", Span::call_site()),
                                arguments: PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                    colon2_token: None,
                                    lt_token: token::Lt { spans: [Span::call_site()] },
                                    args: punctuated::Punctuated::from_iter(vec![GenericArgument::Type(Type::Path(TypePath {
                                        qself: None,
                                        path: syn::Path {
                                            leading_colon: None,
                                            segments: punctuated::Punctuated::from_iter(vec![PathSegment {
                                                ident: Ident::new("i32", Span::call_site()),
                                                arguments: PathArguments::None,
                                            }]),
                                        },
                                    }))]),
                                    gt_token: token::Gt { spans: [Span::call_site()] },
                                }),
                            }]),
                        };
                        let bounds = punctuated::Punctuated::from_iter(vec![TypeParamBound::Trait(TraitBound {
                            paren_token: None,
                            modifier: TraitBoundModifier::None,
                            lifetimes: None,
                            path,
                        })]);
                        *return_type = Box::new(Type::TraitObject(TypeTraitObject {
                            dyn_token: Some(dyn_token),
                            bounds,
                        }));
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets function definitions that return closures, specifically those where the return type of the closure is specified using the `impl Trait` syntax. The operator replaces the return type of the closure with an interface type, such as a trait object, to test how the compiler handles the change in the type system."
    }
}