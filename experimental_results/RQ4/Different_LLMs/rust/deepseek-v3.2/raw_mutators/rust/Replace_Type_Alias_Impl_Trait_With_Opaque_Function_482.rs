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

pub struct Replace_Type_Alias_Impl_Trait_With_Opaque_Function_482;

impl Mutator for Replace_Type_Alias_Impl_Trait_With_Opaque_Function_482 {
    fn name(&self) -> &str {
        "Replace_Type_Alias_Impl_Trait_With_Opaque_Function_482"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut replacements = Vec::new();
        let mut visitor = TypeAliasVisitor {
            aliases: Vec::new(),
        };
        visitor.visit_file(file);
        for alias_info in visitor.aliases {
            let mut helper_name = alias_info.alias_ident.clone();
            helper_name = Ident::new(&format!("{}_helper", helper_name), helper_name.span());
            let mut generic_params = alias_info.generic_params.clone();
            let mut helper_args = Punctuated::<FnArg, Comma>::new();
            let mut call_args = Punctuated::<Expr, Comma>::new();
            let mut expr = alias_info.expr.clone();
            let mut used_idents = HashSet::new();
            collect_idents(&expr, &mut used_idents);
            for ident in used_idents {
                if ident != alias_info.alias_ident {
                    let arg_name = Ident::new(&format!("arg_{}", ident), ident.span());
                    helper_args.push(parse_quote!(#arg_name: _));
                    call_args.push(parse_quote!(#ident));
                }
            }
            let helper_fn: ItemFn = parse_quote! {
                fn #helper_name<#generic_params>(#helper_args) -> impl #alias_info.impl_trait {
                    #expr
                }
            };
            replacements.push((alias_info.alias_stmt_index, alias_info.let_stmt_index, helper_fn, call_args, helper_name, alias_info.generic_args));
        }
        replacements.sort_by(|a, b| b.0.cmp(&a.0));
        for (alias_idx, let_idx, helper_fn, call_args, helper_name, generic_args) in replacements {
            if let Some(Item::Fn(parent_fn)) = file.items.iter_mut().find(|item| {
                if let Item::Fn(f) = item {
                    f.block.stmts.len() > alias_idx && f.block.stmts.len() > let_idx
                } else {
                    false
                }
            }) {
                parent_fn.block.stmts.remove(alias_idx);
                if let Some(Stmt::Local(local)) = parent_fn.block.stmts.get_mut(let_idx) {
                    if let Pat::Type(pat_type) = &mut local.pat {
                        pat_type.ty = Box::new(Type::Infer(TypeInfer { underscore_token: Default::default() }));
                    }
                    local.init = Some((token::Eq::default(), Box::new(parse_quote!(#helper_name::<#generic_args>(#call_args)))));
                }
                parent_fn.block.stmts.insert(let_idx, Stmt::Item(Item::Fn(helper_fn)));
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator transforms type alias impl trait definitions into equivalent opaque functions. For each `type Alias<Params> = impl Trait;` followed by a `let var: Alias<Args> = expr;`, it creates a helper function `fn helper<Params>(...) -> impl Trait { expr }` and replaces the let binding with `let var = helper<Args>(...);`. This mutation alters the handling of generics and opaque types, potentially triggering elaboration drop errors or lifetime resolution issues due to the shift from type alias to function-level opaque type."
    }
}

struct TypeAliasVisitor {
    aliases: Vec<AliasInfo>,
}

struct AliasInfo {
    alias_ident: Ident,
    generic_params: Punctuated<GenericParam, Comma>,
    impl_trait: TypeImplTrait,
    expr: Expr,
    alias_stmt_index: usize,
    let_stmt_index: usize,
    generic_args: PathArguments,
}

impl<'ast> Visit<'ast> for TypeAliasVisitor {
    fn visit_item_fn(&mut self, i: &'ast ItemFn) {
        let mut alias_map = Vec::new();
        for (idx, stmt) in i.block.stmts.iter().enumerate() {
            if let Stmt::Item(Item::Type(type_item)) = stmt {
                if let Type::ImplTrait(impl_trait) = &type_item.ty {
                    alias_map.push((idx, type_item.ident.clone(), type_item.generics.params.clone(), impl_trait.clone()));
                }
            }
        }
        for (alias_idx, alias_ident, generic_params, impl_trait) in alias_map {
            for (let_idx, stmt) in i.block.stmts.iter().enumerate().skip(alias_idx + 1) {
                if let Stmt::Local(local) = stmt {
                    if let Some((_, init_expr)) = &local.init {
                        if let Pat::Type(pat_type) = &local.pat {
                            if let Type::Path(type_path) = &*pat_type.ty {
                                if type_path.path.segments.len() == 1 && type_path.path.segments[0].ident == alias_ident {
                                    let generic_args = type_path.path.segments[0].arguments.clone();
                                    self.aliases.push(AliasInfo {
                                        alias_ident: alias_ident.clone(),
                                        generic_params: generic_params.clone(),
                                        impl_trait: impl_trait.clone(),
                                        expr: init_expr.as_ref().clone(),
                                        alias_stmt_index: alias_idx,
                                        let_stmt_index: let_idx,
                                        generic_args,
                                    });
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
        syn::visit::visit_item_fn(self, i);
    }
}

fn collect_idents(expr: &Expr, set: &mut HashSet<Ident>) {
    match expr {
        Expr::Path(expr_path) => {
            if let Some(ident) = expr_path.path.get_ident() {
                set.insert(ident.clone());
            }
        }
        Expr::Tuple(expr_tuple) => {
            for elem in &expr_tuple.elems {
                collect_idents(elem, set);
            }
        }
        Expr::Array(expr_array) => {
            for elem in &expr_array.elems {
                collect_idents(elem, set);
            }
        }
        Expr::Struct(expr_struct) => {
            for field in &expr_struct.fields {
                collect_idents(&field.expr, set);
            }
        }
        Expr::Binary(expr_binary) => {
            collect_idents(&expr_binary.left, set);
            collect_idents(&expr_binary.right, set);
        }
        Expr::Unary(expr_unary) => {
            collect_idents(&expr_unary.expr, set);
        }
        Expr::Assign(expr_assign) => {
            collect_idents(&expr_assign.left, set);
            collect_idents(&expr_assign.right, set);
        }
        Expr::AssignOp(expr_assign_op) => {
            collect_idents(&expr_assign_op.left, set);
            collect_idents(&expr_assign_op.right, set);
        }
        Expr::Field(expr_field) => {
            collect_idents(&expr_field.base, set);
        }
        Expr::Index(expr_index) => {
            collect_idents(&expr_index.expr, set);
            collect_idents(&expr_index.index, set);
        }
        Expr::Range(expr_range) => {
            if let Some(start) = &expr_range.start {
                collect_idents(start, set);
            }
            if let Some(end) = &expr_range.end {
                collect_idents(end, set);
            }
        }
        Expr::Call(expr_call) => {
            collect_idents(&expr_call.func, set);
            for arg in &expr_call.args {
                collect_idents(arg, set);
            }
        }
        Expr::MethodCall(expr_method_call) => {
            collect_idents(&expr_method_call.receiver, set);
            for arg in &expr_method_call.args {
                collect_idents(arg, set);
            }
        }
        Expr::Paren(expr_paren) => {
            collect_idents(&expr_paren.expr, set);
        }
        Expr::Block(expr_block) => {
            for stmt in &expr_block.stmts {
                if let Stmt::Expr(expr, _) = stmt {
                    collect_idents(expr, set);
                }
            }
        }
        _ => {}
    }
}