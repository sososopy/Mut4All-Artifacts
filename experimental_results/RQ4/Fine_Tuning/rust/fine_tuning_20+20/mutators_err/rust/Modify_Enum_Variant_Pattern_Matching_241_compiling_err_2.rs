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

pub struct Modify_Enum_Variant_Pattern_Matching_241;

impl Mutator for Modify_Enum_Variant_Pattern_Matching_241 {
    fn name(&self) -> &str {
        "Modify_Enum_Variant_Pattern_Matching_241"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut visitor = EnumPatternVisitor {
                    enum_names: HashSet::new(),
                };
                visitor.visit_item_fn(item_fn);
                let enum_names = visitor.enum_names;
                let mut replacer = EnumPatternReplacer { enum_names };
                replacer.visit_item_fn_mut(item_fn);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets enum pattern matching in `if let` and `match` statements. It identifies patterns that match enum variants and modifies them to introduce a type mismatch. Specifically, it replaces literals in the pattern with a different type, such as changing a string literal to an integer literal. This transformation exploits potential weaknesses in the compiler's pattern matching logic by creating deliberate type mismatches, which can lead to errors or unexpected behavior during compilation."
    }
}

struct EnumPatternVisitor {
    enum_names: HashSet<String>,
}

impl<'ast> Visit<'ast> for EnumPatternVisitor {
    fn visit_item_enum(&mut self, node: &'ast ItemEnum) {
        self.enum_names.insert(node.ident.to_string());
    }
}

struct EnumPatternReplacer {
    enum_names: HashSet<String>,
}

impl VisitMut for EnumPatternReplacer {
    fn visit_expr_let_mut(&mut self, node: &mut ExprLet) {
        if let Pat::TupleStruct(pat_tuple_struct) = &*node.pat {
            if let Some(path_segment) = pat_tuple_struct.path.segments.first() {
                if self.enum_names.contains(&path_segment.ident.to_string()) {
                    if let Some(pat) = pat_tuple_struct.elems.first_mut() {
                        *pat = Pat::Lit(PatLit {
                            attrs: Vec::new(),
                            expr: Box::new(Expr::Lit(ExprLit {
                                attrs: Vec::new(),
                                lit: Lit::Int(LitInt::new("42", Span::call_site())),
                            })),
                        });
                    }
                }
            }
        }
    }

    fn visit_expr_match_mut(&mut self, node: &mut ExprMatch) {
        for arm in &mut node.arms {
            if let Pat::TupleStruct(pat_tuple_struct) = &arm.pat {
                if let Some(path_segment) = pat_tuple_struct.path.segments.first() {
                    if self.enum_names.contains(&path_segment.ident.to_string()) {
                        if let Some(pat) = pat_tuple_struct.elems.first_mut() {
                            *pat = Pat::Lit(PatLit {
                                attrs: Vec::new(),
                                expr: Box::new(Expr::Lit(ExprLit {
                                    attrs: Vec::new(),
                                    lit: Lit::Int(LitInt::new("42", Span::call_site())),
                                })),
                            });
                        }
                    }
                }
            }
        }
    }
}