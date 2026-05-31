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

pub struct Replace_Const_Arg_With_FunctionName_432;

impl Mutator for Replace_Const_Arg_With_FunctionName_432 {
    fn name(&self) -> &str {
        "Replace_Const_Arg_With_FunctionName_432"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut functions = Vec::new();
        for item in &file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.constness.is_none() {
                    functions.push(func.sig.ident.clone());
                }
            }
        }
        if functions.is_empty() {
            return;
        }

        struct ReplaceConstArgVisitor {
            functions: Vec<Ident>,
        }

        impl ReplaceConstArgVisitor {
            fn replace_expr(&self, expr: &mut Expr) {
                if let Some(func_name) = self.functions.first() {
                    let new_expr = Expr::Path(ExprPath {
                        attrs: vec![],
                        qself: None,
                        path: SynPath::from(Ident::new(&func_name.to_string(), expr.span())),
                    });
                    *expr = new_expr;
                }
            }
        }

        impl VisitMut for ReplaceConstArgVisitor {
            fn visit_type_array_mut(&mut self, i: &mut TypeArray) {
                self.replace_expr(&mut i.len);
                syn::visit_mut::visit_type_array_mut(self, i);
            }

            fn visit_generic_argument_mut(&mut self, arg: &mut GenericArgument) {
                if let GenericArgument::Const(expr) = arg {
                    self.replace_expr(expr);
                }
                syn::visit_mut::visit_generic_argument_mut(self, arg);
            }

            fn visit_path_arguments_mut(&mut self, args: &mut PathArguments) {
                if let PathArguments::AngleBracketed(args) = args {
                    for arg in &mut args.args {
                        if let GenericArgument::Const(expr) = arg {
                            self.replace_expr(expr);
                        }
                    }
                }
                syn::visit_mut::visit_path_arguments_mut(self, args);
            }
        }

        let mut visitor = ReplaceConstArgVisitor { functions };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces const generic arguments (e.g., array sizes, struct instantiations) with the name of an existing non-const function. This forces the compiler to treat a function name as a const value, potentially exposing errors in const evaluation and type-checking of generic parameters. It leverages existing program structure to create invalid but syntactically valid code, maximizing code coverage by affecting diverse const contexts."
    }
}