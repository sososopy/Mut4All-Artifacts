use syn::{File, Item, ItemFn, ItemImpl, ImplItem, FnArg, Block, WherePredicate, Type, TypeParamBound, TraitBound, TraitBoundModifier, Path, PathSegment, Ident, punctuated::Punctuated, PathArguments};
use proc_macro2::Span;
use crate::mutator::Mutator;

struct Modify_Generic_Constraint_Operator_392;

impl Mutator for Modify_Generic_Constraint_Operator_392 {
    fn name(&self) -> &str {
        "Modify_Generic_Constraint_Operator_392"
    }

    fn chain_of_thought(&self) -> &str {
        "Modify generic constraint operator"
    }

    fn mutate(&self, file: &mut File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let WherePredicate::Type(predicate_type) = predicate {
                            if let Type::Path(type_path) = &predicate_type.bounded_ty {
                                let path_segments = &type_path.path.segments;
                                if path_segments.len() > 0 {
                                    let last_segment = path_segments.last().unwrap();
                                    if last_segment.ident == "Iterator" {
                                        let new_bound = TypeParamBound::Trait(TraitBound {
                                            paren_token: None,
                                            modifier: TraitBoundModifier::None,
                                            lifetimes: None,
                                            path: Path {
                                                leading_colon: None,
                                                segments: Punctuated::from_iter(vec![
                                                    PathSegment {
                                                        ident: Ident::new("Clone", Span::call_site()),
                                                        arguments: PathArguments::None,
                                                    },
                                                ]),
                                            },
                                        });
                                        predicate_type.bounds.push(new_bound);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if let Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let ImplItem::Fn(func) = impl_item {
                        if let Some(where_clause) = &mut func.sig.generics.where_clause {
                            for predicate in &mut where_clause.predicates {
                                if let WherePredicate::Type(predicate_type) = predicate {
                                    if let Type::Path(type_path) = &predicate_type.bounded_ty {
                                        let path_segments = &type_path.path.segments;
                                        if path_segments.len() > 0 {
                                            let last_segment = path_segments.last().unwrap();
                                            if last_segment.ident == "Iterator" {
                                                let new_bound = TypeParamBound::Trait(TraitBound {
                                                    paren_token: None,
                                                    modifier: TraitBoundModifier::None,
                                                    lifetimes: None,
                                                    path: Path {
                                                        leading_colon: None,
                                                        segments: Punctuated::from_iter(vec![
                                                            PathSegment {
                                                                ident: Ident::new("Clone", Span::call_site()),
                                                                arguments: PathArguments::None,
                                                            },
                                                        ]),
                                                    },
                                                });
                                                predicate_type.bounds.push(new_bound);
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
}