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

pub struct Insert_Const_Block_With_Undeclared_Constant_111;

impl Mutator for Insert_Const_Block_With_Undeclared_Constant_111 {
    fn name(&self) -> &str {
        "Insert_Const_Block_With_Undeclared_Constant_111"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ConstBlockVisitor::new();
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const generic arguments in function calls, method calls, and type instantiations. It replaces the original const argument with a const block containing a new constant item without explicit type annotation, initialized with the original expression. This transformation introduces an untyped constant declaration within a const block, potentially triggering type inference failures, const evaluation issues, or compiler crashes in const generic processing."
    }
}

struct ConstBlockVisitor {
    used_names: HashSet<String>,
}

impl ConstBlockVisitor {
    fn new() -> Self {
        Self {
            used_names: HashSet::new(),
        }
    }

    fn generate_unique_name(&mut self) -> Ident {
        let mut rng = thread_rng();
        let mut name = format!("CONST_{}", rng.gen_range(1000..9999));
        while self.used_names.contains(&name) {
            name = format!("CONST_{}", rng.gen_range(1000..9999));
        }
        self.used_names.insert(name.clone());
        Ident::new(&name, Span::call_site())
    }
}

impl VisitMut for ConstBlockVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Call(call) = expr {
            self.visit_expr_mut(&mut *call.func);
            for arg in &mut call.args {
                self.visit_expr_mut(arg);
            }
            return;
        }

        if let Expr::MethodCall(method_call) = expr {
            self.visit_expr_mut(&mut method_call.receiver);
            for arg in &mut method_call.args {
                self.visit_expr_mut(arg);
            }
            return;
        }

        if let Expr::Path(path_expr) = expr {
            if let Some(last_segment) = path_expr.path.segments.last_mut() {
                if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                    for arg in &mut args.args {
                        if let GenericArgument::Const(const_arg) = arg {
                            let new_const_name = self.generate_unique_name();
                            let new_const_block: Expr = parse_quote! {
                                const {
                                    const #new_const_name = #const_arg;
                                    #new_const_name
                                }
                            };
                            *arg = GenericArgument::Const(new_const_block);
                        }
                    }
                }
            }
        }

        syn::visit_mut::visit_expr_mut(self, expr);
    }

    fn visit_type_mut(&mut self, ty: &mut Type) {
        if let Type::Path(type_path) = ty {
            if let Some(last_segment) = type_path.path.segments.last_mut() {
                if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                    for arg in &mut args.args {
                        if let GenericArgument::Const(const_arg) = arg {
                            let new_const_name = self.generate_unique_name();
                            let new_const_block: Expr = parse_quote! {
                                const {
                                    const #new_const_name = #const_arg;
                                    #new_const_name
                                }
                            };
                            *arg = GenericArgument::Const(new_const_block);
                        }
                    }
                }
            }
        }

        syn::visit_mut::visit_type_mut(self, ty);
    }
}