use syn::{File, Ident, ImplItem, Type, WherePredicate};
use proc_macro2::{Span};
use crate::mutator::Mutator;

pub struct Modify_Const_Generic_Bounds;

impl Mutator for Modify_Const_Generic_Bounds {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Bounds"
    }

    fn mutate(&self, file: &mut File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                if let Some(where_clause) = &mut func.sig.generics.where_clause {
                    for predicate in &mut where_clause.predicates {
                        if let WherePredicate::Type(predicate_type) = predicate {
                            if let Type::Path(type_path) = &mut predicate_type.bounded_ty {
                                let path = &mut type_path.path;
                                if path.segments.len() == 1 {
                                    let segment = &mut path.segments[0];
                                    if segment.ident ==usize {
                                        segment.ident = Ident::new("isize", Span::call_site());
                                    } else if segment.ident == "isize" {
                                        segment.ident = Ident::new("usize", Span::call_site());
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let ImplItem::Fn(func) = impl_item {
                        if let Some(where_clause) = &mut func.sig.generics.where_clause {
                            for predicate in &mut where_clause.predicates {
                                if let WherePredicate::Type(predicate_type) = predicate {
                                    if let Type::Path(type_path) = &mut predicate_type.bounded_ty {
                                        let path = &mut type_path.path;
                                        if path.segments.len() == 1 {
                                            let segment = &mut path.segments[0];
                                            if segment.ident ==usize {
                                                segment.ident = Ident::new("isize", Span::call_site());
                                            } else if segment.ident == "isize" {
                                                segment.ident = Ident::new("usize", Span::call_site());
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
        "The mutation operator modifies the bounds of const generics in function and method signatures. It changes the type of the bound from usize to isize or vice versa, testing the compiler's handling of const generics and their application."
    }
}