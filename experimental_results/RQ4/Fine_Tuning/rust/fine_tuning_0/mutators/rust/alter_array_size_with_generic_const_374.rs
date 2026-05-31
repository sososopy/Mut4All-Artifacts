use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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

pub struct Alter_Array_Size_With_Generic_Const_374;

impl Mutator for Alter_Array_Size_With_Generic_Const_374 {
    fn name(&self) -> &str {
        "Alter_Array_Size_With_Generic_Const_374"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Array(type_array) = &mut *pat_type.ty {
                            if let Expr::Block(expr_block) = &type_array.len {
                                if let Some(expr) = expr_block.block.stmts.first() {
                                    if let Stmt::Expr(Expr::Path(expr_path), _) = expr {
                                        if expr_path.path.segments.len() == 1 {
                                            let segment = &expr_path.path.segments[0];
                                            if segment.ident == "N" {
                                                let new_expr: Expr = parse_quote! { N + 1 };
                                                type_array.len = new_expr;
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
        "The mutation operator identifies function parameters that are arrays with sizes determined by a const expression involving a generic const parameter. It modifies the const expression to include additional arithmetic operations, such as adding 1, to test const evaluation and arithmetic handling in the compiler."
    }
}