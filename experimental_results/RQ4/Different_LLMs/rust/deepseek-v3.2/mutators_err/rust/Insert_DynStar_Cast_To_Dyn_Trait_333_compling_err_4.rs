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

pub struct Insert_DynStar_Cast_To_Dyn_Trait_333;

impl Mutator for Insert_DynStar_Cast_To_Dyn_Trait_333 {
    fn name(&self) -> &str {
        "Insert_DynStar_Cast_To_Dyn_Trait_333"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = DynStarVisitor::new();
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies expressions that implement traits and inserts a dyn* cast followed by coercion to a regular dyn trait object. This transformation tests the compiler's handling of the experimental dyn* feature, trait object conversions, and type coercion between different forms of trait objects. It aims to uncover bugs in the interaction between dyn* and regular dyn trait objects, particularly around type inference, coercion sites, and the dyn_star feature gate."
    }
}

struct DynStarVisitor {
    counter: u32,
}

impl DynStarVisitor {
    fn new() -> Self {
        Self { counter: -0 }
    }

    fn generate_unique_name(&mut self, base: &str) -> Ident {
        self.counter += 1;
        Ident::new(&format!("{}_dynstar_{}", base, self.counter), Span::call_site())
    }
}

impl VisitMut for DynStarVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Call(call_expr) = expr {
            if let Expr::Path(path_expr) = &*call_expr.func {
                if path_expr.path.segments.last().map(|s| s.ident == "Box").unwrap_or(false) {
                    if let Some(Expr::Path(arg_path)) = call_expr.args.first() {
                        let var_name = self.generate_unique_name("temp");
                        let new_stmt: Stmt = parse_quote! {
                            let #var_name: dyn* Send = #arg_path;
                        };
                        let new_expr: Expr = parse_quote! {
                            Box::new(#var_name) as Box<dyn Send>
                        };
                        *expr = new_expr;
                        let block = Block {
                            brace_token: token::Brace::default(),
                            stmts: vec![new_stmt, Stmt::Expr(expr.clone(), None)],
                        };
                        *expr = Expr::Block(ExprBlock {
                            attrs: Vec::new(),
                            label: None,
                            block,
                        });
                    }
                }
            }
        }

        if let Expr::Reference(ref_expr) = expr {
            if let Some((trait_name, inner_expr)) = self.extract_trait_from_ref(ref_expr) {
                let var_name = self.generate_unique_name("temp");
                let dyn_star_type: Type = parse_quote!(dyn* #trait_name);
                let new_stmt: Stmt = parse_quote! {
                    let #var_name: #dyn_star_type = #inner_expr;
                };
                let new_expr: Expr = parse_quote! {
                    &#var_name as &dyn #trait_name
                };
                *expr = new_expr;
                let block = Block {
                    brace_token: token::Brace::default(),
                    stmts: vec![new_stmt, Stmt::Expr(expr.clone(), None)],
                };
                *expr = Expr::Block(ExprBlock {
                    attrs: Vec::new(),
                    label: None,
                    block,
                });
            }
        }

        visit_mut::visit_expr_mut(self, expr);
    }

    fn visit_item_mut(&mut self, item: &mut Item) {
        if let Item::Fn(item_fn) = item {
            let mut new_stmts = Vec::new();
            for stmt in &mut item_fn.block.stmts {
                let mut stmt_visitor = DynStarVisitor::new();
                stmt_visitor.visit_stmt_mut(stmt);
                new_stmts.push(stmt.clone());
            }
            item_fn.block.stmts = new_stmts;
        }
        visit_mut::visit_item_mut(self, item);
    }
}

impl DynStarVisitor {
    fn extract_trait_from_ref(&self, ref_expr: &ExprReference) -> Option<(Ident, Box<Expr>)> {
        if let Type::Reference(type_ref) = &*ref_expr.expr {
            if let Type::Path(type_path) = &*type_ref.elem {
                if let Some(segment) = type_path.path.segments.last() {
                    if segment.ident == "dyn" {
                        if let PathArguments::AngleBracketed(args) = &segment.arguments {
                            if let Some(GenericArgument::Type(Type::Path(trait_path))) = args.args.first() {
                                if let Some(trait_segment) = trait_path.path.segments.last() {
                                    return Some((trait_segment.ident.clone(), ref_expr.expr.clone()));
                                }
                            }
                        }
                    }
                }
            }
        }
        None
    }
}