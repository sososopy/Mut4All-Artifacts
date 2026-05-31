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

pub struct Replace_Transmutability_Assertion_With_Invalid_Discriminant_Transmute_465;

impl Mutator for Replace_Transmutability_Assertion_With_Invalid_Discriminant_Transmute_465 {
    fn name(&self) -> &str {
        "Replace_Transmutability_Assertion_With_Invalid_Discriminant_Transmute_465"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = AssertionVisitor::default();
        visitor.visit_file_mut(file);
        file.items.extend(visitor.new_items);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets transmutability assertions using std::mem::BikeshedIntrinsicFrom. It identifies the destination enum type, creates a union with matching discriminant representation, and replaces the assertion with a transmutation using an out-of-bounds variant index. This transformation directly tests discriminant handling logic for variant index overflow scenarios, potentially exposing layout compatibility bugs and unsafe transmute edge cases."
    }
}

#[derive(Default)]
struct AssertionVisitor {
    new_items: Vec<Item>,
    encountered_enums: HashSet<Ident>,
}

impl VisitMut for AssertionVisitor {
    fn visit_expr_call_mut(&mut self, node: &mut ExprCall) {
        if let Expr::Path(ExprPath { path, .. }) = &*node.func {
            if is_transmutation_assertion(path) {
                if let Some((src_type, dst_type)) = extract_types_from_assertion(&node.args) {
                    if let Type::Path(TypePath { path: dst_path, .. }) = &*dst_type {
                        if let Some(dst_ident) = dst_path.get_ident() {
                            if self.encountered_enums.contains(dst_ident) {
                                if let Some(discriminant_type) = get_discriminant_type(dst_ident, &self.new_items) {
                                    if let Some(variant_count) = get_variant_count(dst_ident, &self.new_items) {
                                        let invalid_index = variant_count + 1;
                                        let union_name = Ident::new(&format!("Discriminant_{}", dst_ident), Span::call_site());
                                        
                                        if !self.new_items.iter().any(|item| {
                                            if let Item::Union(u) = item {
                                                u.ident == union_name
                                            } else {
                                                false
                                            }
                                        }) {
                                            let union_item: Item = parse_quote! {
                                                #[repr(C)]
                                                union #union_name {
                                                    d: #discriminant_type,
                                                }
                                            };
                                            self.new_items.push(union_item);
                                        }
                                        
                                        let replacement: ExprCall = parse_quote! {
                                            {
                                                let invalid_union = #union_name { d: #invalid_index };
                                                let _: #dst_path = unsafe { std::mem::transmute(invalid_union) };
                                            }
                                        };
                                        *node = replacement;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_expr_call_mut(self, node);
    }
    
    fn visit_item_mut(&mut self, node: &mut Item) {
        if let Item::Enum(item_enum) = node {
            self.encountered_enums.insert(item_enum.ident.clone());
        }
        syn::visit_mut::visit_item_mut(self, node);
    }
}

fn is_transmutation_assertion(path: &SynPath) -> bool {
    let segments: Vec<String> = path.segments.iter().map(|seg| seg.ident.to_string()).collect();
    segments.len() >= 2 && (segments.last() == Some(&"is_transmutable".to_string()) || segments.contains(&"BikeshedIntrinsicFrom".to_string()))
}

fn extract_types_from_assertion(args: &Punctuated<Expr, Comma>) -> Option<(Box<Type>, Box<Type>)> {
    if args.len() == 1 {
        if let Expr::Path(ExprPath { path, .. }) = &args[0] {
            if let PathArguments::AngleBracketed(angle_args) = &path.segments.last()?.arguments {
                if angle_args.args.len() >= 2 {
                    if let (GenericArgument::Type(src_type), GenericArgument::Type(dst_type)) = (&angle_args.args[0], &angle_args.args[1]) {
                        return Some((Box::new(src_type.clone()), Box::new(dst_type.clone())));
                    }
                }
            }
        }
    }
    None
}

fn get_discriminant_type(enum_ident: &Ident, items: &[Item]) -> Option<Type> {
    for item in items {
        if let Item::Enum(item_enum) = item {
            if item_enum.ident == *enum_ident {
                if let Some((_, expr)) = item_enum.discriminant.as_ref() {
                    if let Expr::Lit(ExprLit { lit: Lit::Int(int_lit), .. }) = expr {
                        let suffix = int_lit.suffix();
                        match suffix {
                            "u8" => return Some(parse_quote!(u8)),
                            "u16" => return Some(parse_quote!(u16)),
                            "u32" => return Some(parse_quote!(u32)),
                            "u64" => return Some(parse_quote!(u64)),
                            "u128" => return Some(parse_quote!(u128)),
                            "usize" => return Some(parse_quote!(usize)),
                            _ => None,
                        }
                    } else {
                        None
                    }
                } else {
                    return Some(parse_quote!(u8));
                }
            }
        }
    }
    None
}

fn get_variant_count(enum_ident: &Ident, items: &[Item]) -> Option<u64> {
    for item in items {
        if let Item::Enum(item_enum) = item {
            if item_enum.ident == *enum_ident {
                return Some(item_enum.variants.len() as u64);
            }
        }
    }
    None
}