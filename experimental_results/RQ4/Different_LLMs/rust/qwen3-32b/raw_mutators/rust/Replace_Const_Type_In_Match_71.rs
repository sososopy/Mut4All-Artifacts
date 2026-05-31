use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprMatch, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemConst, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatPath, Path as SynPath,
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

pub struct Replace_Const_Type_In_Match_71;

impl Mutator for Replace_Const_Type_In_Match_71 {
    fn name(&self) -> &str {
        "Replace_Const_Type_In_Match_71"
    }
    fn mutate(&self, file: &mut syn::File) {
        // Collect all constants in the file
        let mut constants_map = std::collections::HashMap::new();
        for item in &mut file.items {
            if let Item::Const(item_const) = item {
                constants_map.insert(&item_const.ident, item_const);
            }
        }

        // Visitor to find match arms with path patterns referring to constants
        struct MatchArmVisitor<'a> {
            constants: std::collections::HashMap<&'a Ident, &'a mut ItemConst>,
            constants_to_modify: Vec<&'a mut ItemConst>,
        }

        impl<'a> MatchArmVisitor<'a> {
            fn new(constants: std::collections::HashMap<&'a Ident, &'a mut ItemConst>) -> Self {
                Self {
                    constants,
                    constants_to_modify: Vec::new(),
                }
            }
        }

        impl<'a> VisitMut for MatchArmVisitor<'a> {
            fn visit_expr_match_mut(&mut self, expr: &mut ExprMatch) {
                for arm in &mut expr.arms {
                    if let Pat::Path(pat_path) = &arm.pat {
                        if let Some(ident) = pat_path.path.get_ident() {
                            if let Some(constant) = self.constants.get(&ident) {
                                // Avoid adding duplicates
                                if !self.constants_to_modify.contains(&constant) {
                                    self.constants_to_modify.push(constant);
                                }
                            }
                        }
                    }
                }
            }
        }

        let mut visitor = MatchArmVisitor::new(constants_map);
        visitor.visit_file_mut(file);

        // Modify the collected constants
        for constant in visitor.constants_to_modify {
            // Change type to i32 and value to 0
            constant.ty = parse_quote!(i32);
            if let Some(expr) = &mut constant.expr {
                *expr = parse_quote!(0);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies constants used in match arm patterns and replaces their type with an incompatible one (e.g., from unit type `()` to `i32`). This creates a type mismatch between the constant's definition and the match expression's expected type, testing the compiler's ability to handle invalid pattern matching scenarios and type coherence errors."
    }
}