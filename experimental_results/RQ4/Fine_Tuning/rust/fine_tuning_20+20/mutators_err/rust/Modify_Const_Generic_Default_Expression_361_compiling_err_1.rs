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

pub struct Modify_Const_Generic_Default_Expression_361;

impl Mutator for Modify_Const_Generic_Default_Expression_361 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Default_Expression_361"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct ConstGenericVisitor {
            consts: HashSet<String>,
        }
        impl<'ast> Visit<'ast> for ConstGenericVisitor {
            fn visit_item_const(&mut self, i: &'ast ItemConst) {
                self.consts.insert(i.ident.to_string());
            }
        }
        let mut visitor = ConstGenericVisitor {
            consts: HashSet::new(),
        };
        visitor.visit_file(file);
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut new_generics = item_struct.generics.clone();
                for param in &mut new_generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Some(default) = &const_param.default {
                            if let Expr::Path(expr_path) = &**default {
                                if expr_path.path.segments.len() == 1 {
                                    let segment = expr_path.path.segments.first().unwrap();
                                    if visitor.consts.contains(&segment.ident.to_string()) {
                                        let new_expr: Expr = parse_quote!(#expr_path + 1);
                                        const_param.default = Some(new_expr);
                                    }
                                }
                            }
                        }
                    }
                }
                item_struct.generics = new_generics;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets structs with const generic parameters that have default values specified as constants. It replaces the constant default with an expression that modifies the constant, such as adding one. This transformation leverages the const generic system's reliance on const expressions and aims to expose weaknesses in how the compiler handles default const values, particularly in edge cases involving const arithmetic and type checking."
    }
}