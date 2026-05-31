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

pub struct Alter_Enum_Variants_With_Arity_To_Mismatched_Types_244;

impl Mutator for Alter_Enum_Variants_With_Arity_To_Mismatched_Types_244 {
    fn name(&self) -> &str {
        "Alter_Enum_Variants_With_Arity_To_Mismatched_Types_244"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Enum(item_enum) = item {
                let mut variant_arities = Vec::new();
                for variant in &item_enum.variants {
                    let arity = match &variant.fields {
                        Fields::Named(fields_named) => fields_named.named.len(),
                        Fields::Unnamed(fields_unnamed) => fields_unnamed.unnamed.len(),
                        Fields::Unit => 0,
                    };
                    variant_arities.push(arity);
                }
                let mut rng = thread_rng();
                let new_arity = if let Some(max_arity) = variant_arities.iter().max() {
                    rng.gen_range(0..=*max_arity + 2)
                } else {
                    0
                };
                let new_variant_ident = Ident::new(
                    &format!("Mutant{}", item_enum.variants.len()),
                    Span::call_site(),
                );
                let new_variant = match new_arity {
                    0 => Variant {
                        attrs: Vec::new(),
                        ident: new_variant_ident,
                        fields: Fields::Unit,
                        discriminant: None,
                    },
                    _ => Variant {
                        attrs: Vec::new(),
                        ident: new_variant_ident,
                        fields: Fields::Unnamed(FieldsUnnamed {
                            paren_token: token::Paren::default(),
                            unnamed: (0..new_arity)
                                .map(|_| {
                                    Field::parse_named
                                        .parse2(quote! { _: () })
                                        .unwrap_or_else(|_| Field {
                                            attrs: Vec::new(),
                                            vis: Visibility::Inherited,
                                            ident: None,
                                            colon_token: None,
                                            ty: Type::Tuple(TypeTuple {
                                                paren_token: token::Paren::default(),
                                                elems: Punctuated::new(),
                                            }),
                                        })
                                })
                                .collect(),
                        }),
                        discriminant: None,
                    },
                };
                item_enum.variants.push(new_variant);
            }
        }
        let mut enum_usages = HashSet::new();
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut visitor = EnumUsageVisitor {
                    enum_usages: &mut enum_usages,
                };
                visitor.visit_item_fn(item_fn);
            }
        }
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let Some(mutator) = EnumPatternMutator::new(item_fn, &enum_usages) {
                    mutator.mutate();
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets enums with multiple variants of different arities that are used in pattern matching. It adds a new variant with a randomly chosen arity and modifies an existing pattern match to use the new variant with a mismatched arity, potentially triggering bugs related to pattern arity analysis in the Rust compiler."
    }
}

struct EnumUsageVisitor<'a> {
    enum_usages: &'a mut HashSet<String>,
}

impl<'a> Visit<'a> for EnumUsageVisitor<'a> {
    fn visit_expr_match(&mut self, node: &'a ExprMatch) {
        if let Expr::Path(ExprPath { path, .. }) = &*node.expr {
            if let Some(segment) = path.segments.last() {
                self.enum_usages.insert(segment.ident.to_string());
            }
        }
        syn::visit::visit_expr_match(self, node);
    }
}

struct EnumPatternMutator<'a> {
    item_fn: &'a mut ItemFn,
    enum_usages: &'a HashSet<String>,
}

impl<'a> EnumPatternMutator<'a> {
    fn new(item_fn: &'a mut ItemFn, enum_usages: &'a HashSet<String>) -> Option<Self> {
        if item_fn.sig.ident != "main" {
            Some(Self {
                item_fn,
                enum_usages,
            })
        } else {
            None
        }
    }

    fn mutate(&mut self) {
        let mut rng = thread_rng();
        let mut target_match_indices = Vec::new();
        for (index, stmt) in self.item_fn.block.stmts.iter().enumerate() {
            if let Stmt::Expr(Expr::Match(expr_match), _) = stmt {
                if let Expr::Path(ExprPath { path, .. }) = &*expr_match.expr {
                    if let Some(segment) = path.segments.last() {
                        if self.enum_usages.contains(&segment.ident.to_string()) {
                            target_match_indices.push(index);
                        }
                    }
                }
            }
        }
        if target_match_indices.is_empty() {
            return;
        }
        let target_match_index = *target_match_indices.choose(&mut rng).unwrap();
        let target_match = match &mut self.item_fn.block.stmts[target_match_index] {
            Stmt::Expr(Expr::Match(expr_match), _) => expr_match,
            _ => return,
        };
        let mut variant_arities = Vec::new();
        for arm in &target_match.arms {
            if let Pat::TupleStruct(pat_tuple_struct) = &arm.pat {
                variant_arities.push(pat_tuple_struct.elems.len());
            } else {
                variant_arities.push(0);
            }
        }
        let new_arity = if let Some(max_arity) = variant_arities.iter().max() {
            rng.gen_range(0..=*max_arity + 2)
        } else {
            0
        };
        let new_variant_ident = Ident::new(
            &format!("Mutant{}", target_match.arms.len()),
            Span::call_site(),
        );
        let new_arm = match new_arity {
            0 => parse_quote! {
                #new_variant_ident => {}
            },
            _ => {
                let wildcards: Vec<_> = (0..new_arity).map(|_| quote! { _ }).collect();
                parse_quote! {
                    #new_variant_ident(#(#wildcards),*) => {}
                }
            }
        };
        target_match.arms.push(new_arm);
    }
}