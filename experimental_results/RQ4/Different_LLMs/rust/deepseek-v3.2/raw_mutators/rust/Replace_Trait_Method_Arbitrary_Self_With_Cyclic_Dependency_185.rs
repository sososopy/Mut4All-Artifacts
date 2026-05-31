use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Replace_Trait_Method_Arbitrary_Self_With_Cyclic_Dependency_185;

impl Mutator for Replace_Trait_Method_Arbitrary_Self_With_Cyclic_Dependency_185 {
    fn name(&self) -> &str {
        "Replace_Trait_Method_Arbitrary_Self_With_Cyclic_Dependency_185"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                if trait_item.generics.params.len() > 0 && trait_item.items.len() > 0 {
                    let mut rng = thread_rng();
                    let method_index = rng.gen_range(0..trait_item.items.len());
                    if let TraitItem::Method(method) = &mut trait_item.items[method_index] {
                        let generic_param = trait_item.generics.params.first().unwrap();
                        let generic_ident = match generic_param {
                            GenericParam::Type(type_param) => &type_param.ident,
                            GenericParam::Lifetime(lifetime_param) => continue,
                            GenericParam::Const(const_param) => continue,
                        };
                        
                        let trait_name = &trait_item.ident;
                        
                        if let FnArg::Receiver(receiver) = &method.sig.inputs.first().unwrap() {
                            if receiver.reference.is_none() && receiver.mutability.is_none() && receiver.colon_token.is_none() {
                                method.sig.inputs[0] = parse_quote!(self: #generic_ident);
                            }
                        }
                        
                        method.sig.output = ReturnType::Type(
                            token::RArrow::default(),
                            Box::new(Type::TraitObject(TypeTraitObject {
                                dyn_token: Some(token::Dyn::default()),
                                bounds: Punctuated::from_iter(vec![TypeParamBound::Trait(TraitBound {
                                    paren_token: None,
                                    modifier: TraitBoundModifier::None,
                                    lifetimes: None,
                                    path: SynPath {
                                        leading_colon: None,
                                        segments: Punctuated::from_iter(vec![PathSegment {
                                            ident: trait_name.clone(),
                                            arguments: PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                                colon2_token: None,
                                                lt_token: token::Lt::default(),
                                                args: Punctuated::from_iter(vec![GenericArgument::Type(Type::Path(TypePath {
                                                    qself: None,
                                                    path: SynPath::from(generic_ident.clone()),
                                                }))]),
                                                gt_token: token::Gt::default(),
                                            }),
                                        }]),
                                    },
                                })]),
                            }))
                        );
                        
                        for param in &mut trait_item.generics.params {
                            if let GenericParam::Type(type_param) = param {
                                let dispatch_bound = TraitBound {
                                    paren_token: None,
                                    modifier: TraitBoundModifier::None,
                                    lifetimes: None,
                                    path: SynPath::from(Ident::new("DispatchFromDyn", Span::call_site())),
                                };
                                let deref_bound = TraitBound {
                                    paren_token: None,
                                    modifier: TraitBoundModifier::None,
                                    lifetimes: None,
                                    path: SynPath::from(Ident::new("Deref", Span::call_site())),
                                };
                                type_param.bounds.push(TypeParamBound::Trait(dispatch_bound));
                                type_param.bounds.push(TypeParamBound::Trait(deref_bound));
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait definitions with at least one generic parameter and one method. It modifies a randomly chosen method to use an arbitrary self type based on the trait's first generic parameter, and changes its return type to a dyn trait object referencing the same trait with the generic parameter. Additionally, it adds DispatchFromDyn and Deref bounds to the generic parameter. This creates a cyclic dependency between the self type and return type, potentially triggering layout computation issues similar to the bug report."
    }
}