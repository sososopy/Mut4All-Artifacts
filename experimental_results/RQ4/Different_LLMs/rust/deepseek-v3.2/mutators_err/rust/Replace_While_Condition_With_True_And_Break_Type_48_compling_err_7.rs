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

pub struct Replace_While_Condition_With_True_And_Break_Type_48;

impl Mutator for Replace_While_Condition_With_True_And_Break_Type_48 {
    fn name(&self) -> &str {
        "Replace_While_Condition_With_True_And_Break_Type_48"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = WhileLoopVisitor {
            types_in_program: HashSet::new(),
        };
        visitor.visit_file(file);
        for item in &mut file.items {
            if let Item::Const(item_const) = item {
                let mut visitor = WhileLoopMutator {
                    types_in_program: visitor.types_in_program.clone(),
                };
                visitor.visit_expr_mut(&mut item_const.expr);
            }
        }
        struct WhileLoopVisitor {
            types_in_program: HashSet<String>,
        }
        impl Visit<'_> for WhileLoopVisitor {
            fn visit_expr(&mut self, expr: &Expr) {
                match expr {
                    Expr::Const(expr_const) => {
                        syn::visit::visit_expr(self, expr);
                    }
                    Expr::Lit(expr_lit) => {
                        match &expr_lit.lit {
                            syn::Lit::Int(lit_int) => {
                                if let Some(suffix) = lit_int.suffix() {
                                    self.types_in_program.insert(suffix.to_string());
                                } else {
                                    self.types_in_program.insert("i32".to_string());
                                }
                            }
                            syn::Lit::Float(lit_float) => {
                                if let Some(suffix) = lit_float.suffix() {
                                    self.types_in_program.insert(suffix.to_string());
                                } else {
                                    self.types_in_program.insert("f64".to_string());
                                }
                            }
                            syn::Lit::Bool(_) => {
                                self.types_in_program.insert("bool".to_string());
                            }
                            syn::Lit::Char(_) => {
                                self.types_in_program.insert("char".to_string());
                            }
                            syn::Lit::Str(_) => {
                                self.types_in_program.insert("String".to_string());
                            }
                            syn::Lit::Byte(_) => {
                                self.types_in_program.insert("u8".to_string());
                            }
                            syn::Lit::ByteStr(_) => {
                                self.types_in_program.insert("[u8]".to_string());
                            }
                            syn::Lit::Verbatim(_) => {}
                        }
                        syn::visit::visit_expr(self, expr);
                    }
                    _ => syn::visit::visit_expr(self, expr),
                }
            }
        }
        struct WhileLoopMutator {
            types_in_program: HashSet<String>,
        }
        impl VisitMut for WhileLoopMutator {
            fn visit_expr_mut(&mut self, expr: &mut Expr) {
                match expr {
                    Expr::While(expr_while) => {
                        expr_while.cond = Box::new(Expr::Lit(ExprLit {
                            attrs: Vec::new(),
                            lit: syn::Lit::Bool(LitBool {
                                value: true,
                                span: Span::call_site(),
                            }),
                        }));
                        syn::visit_mut::visit_block_mut(self, &mut expr_while.body);
                        syn::visit_mut::visit_expr_m2ut(self, expr);
                    }
                    Expr::Break(expr_break) => {
                        if let Some(break_expr) = &mut expr_break.expr {
                            let mut new_type = None;
                            let mut new_value = None;
                            match &**break_expr {
                                Expr::Lit(expr_lit) => {
                                    match &expr_lit.lit {
                                        syn::Lit::Int(lit_int) => {
                                            let current_type = if let Some(suffix) = lit_int.suffix() {
                                                suffix.to_string()
                                            } else {
                                                "i32".to_string()
                                            };
                                            let alternative_types = self.types_in_program.iter().filter(|t| *t != current_type).collect::<Vec<_>>();
                                            if alternative_types.len() > 0 {
                                                let chosen_type = alternative_types.choose(&mut thread_rng()).unwrap();
                                                new_type = Some(chosen_type.clone());
                                                match chosen_type.as_str() {
                                                    "i32" => new_value = Some(Expr::Lit(ExprLit {
                                                        attrs: Vec::new(),
                                                        lit: syn::Lit::Int(LitInt::new("0", Span::call_site())),
                                                    })),
                                                    "u32" => new_value = Some(Expr::Lit(ExprLit {
                                                        attrs: Vec::new(),
                                                        lit: syn::Lit::Int(LitInt::new("0u32", Span::call_site())),
                                                    })),
                                                    "i64" => new_value = Some(Expr::Lit(ExprLit {
                                                        attrs: Vec::new(),
                                                        lit: syn::Lit::Int(LitInt::new("0i64", Span::call_site())),
                                                    })),
                                                    "u64" => new_value = Some(Expr::Lit(ExprLit {
                                                        attrs: Vec::new(),
                                                        lit: syn::Lit::Int(LitInt::new("0u64", Span::call_site())),
                                                    })),
                                                    "f32" => new_value = Some(Expr::Lit(ExprLit {
                                                        attrs: Vec::new(),
                                                        lit: syn::Lit::Float(LitFloat::new("0.0f32", Span::call_site())),
                                                    })),
                                                    "f64" => new_value = Some(Expr::Lit(ExprLit {
                                                        attrs: Vec::new(),
                                                        lit: syn::Lit::Float(LitFloat::new("0.0", Span::call_site())),
                                                    })),
                                                    "bool" => new_value = Some(Expr::Lit(ExprLit {
                                                        attrs: Vec::new(),
                                                        lit: syn::Lit::Bool(LitBool {
                                                            value: false,
                                                            span: Span::call_site(),
                                                        }),
                                                    })),
                                                    "char" => new_value = Some(Expr::Lit(ExprLit {
                                                        attrs: Vec::new(),
                                                        lit: syn::Lit::Char(LitChar::new('\'', Span::call_site())),
                                                    })),
                                                    "()" => new_value = Some(Expr::Lit(ExprLit {
                                                        attrs: Vec::new(),
                                                        lit: syn::Lit::Verbatim(proc_macro2::Literal::string("()")),
                                                    })),
                                                    _ => new_value = Some(Expr::Lit(ExprLit {
                                                        attrs: Vec::new(),
                                                        lit: syn::Lit::Verbatim(proc_macro2::Literal::string("()")),
                                                    })),
                                                }
                                            } else {
                                                new_value = Some(Expr::Lit(ExprLit {
                                                    attrs: Vec::new(),
                                                    lit: syn::Lit::Verbatim(proc_macro2::Literal::string("()")),
                                                }));
                                            }
                                        }
                                        syn::Lit::Float(lit_float) => {
                                            let current_type = if let Some(suffix) = lit_float.suffix() {
                                                suffix.to_string()
                                            } else {
                                                "f64".to_string()
                                            };
                                            let alternative_types = self.types_in_program.iter().filter(|t| *t != current_type).collect::<Vec<_>>();
                                            if alternative_types.len() > 0 {
                                                let chosen_type = alternative_types.choose(&mut thread_rng()).unwrap();
                                                new_type = Some(chosen_type.clone());
                                                match chosen_type.as_str() {
                                                    "i32" => new_value = Some(Expr::Lit(ExprLit {
                                                        attrs: Vec::new(),
                                                        lit: syn::Lit::Int(LitInt::new("0", Span::call_site())),
                                                    })),
                                                    "u32" => new_value = Some(Expr::Lit(ExprLit {
                                                        attrs: Vec::new(),
                                                        lit: syn::Lit::Int(LitInt::new("0u32", Span::call_site())),
                                                    })),
                                                    "i64" => new_value = Some(Expr::Lit(ExprLit {
                                                        attrs: Vec::new(),
                                                        lit: syn::Lit::Int(LitInt::new("0i64", Span::call_site())),
                                                    })),
                                                    "u64" => new_value = Some(Expr::Lit(ExprLit {
                                                        attrs: Vec::new(),
                                                        lit: syn::Lit::Int(LitInt::new("0u64", Span::call_site())),
                                                    })),
                                                    "f32" => new_value = Some(Expr::Lit(ExprLit {
                                                        attrs: Vec::new(),
                                                        lit: syn::Lit::Float(LitFloat::new("0.0f32", Span::call_site())),
                                                    })),
                                                    "f64" => new_value = Some(Expr::Lit(ExprLit {
                                                        attrs: Vec::new(),
                                                        lit: syn::Lit::Float(LitFloat::new("0.0", Span::call_site())),
                                                    })),
                                                    "bool" => new_value = Some(Expr::Lit(ExprLit {
                                                        attrs: Vec::new(),
                                                        lit: syn::Lit::Bool(LitBool {
                                                            value: false,
                                                            span: Span::call_site(),
                                                        }),
                                                    })),
                                                    "char" => new_value = Some(Expr::Lit(ExprLit {
                                                        attrs: Vec::new(),
                                                        lit: syn::Lit::Char(LitChar::new('\'', Span::call_site())),
                                                    })),
                                                    "()" => new_value = Some(Expr::Lit(ExprLit {
                                                        attrs: Vec::new(),
                                                        lit: syn::Lit::Verbatim(proc_macro2::Literal::string("()")),
                                                    })),
                                                    _ => new_value = Some(Expr::Lit(ExprLit {
                                                        attrs: Vec::new(),
                                                        lit: syn::Lit::Verbatim(proc_macro2::Literal::string("()")),
                                                    })),
                                                }
                                            } else {
                                                new_value = Some(Expr::Lit(ExprLit {
                                                    attrs: Vec::new(),
                                                    lit: syn::Lit::Verbatim(proc_macro2::Literal::string("()")),
                                                }));
                                            }
                                        }
                                        syn::Lit::Bool(_) => {
                                            let alternative_types = self.types_in_program.iter().filter(|t| *t != "bool").collect::<Vec<_>>();
                                            if alternative_types.len() > 0 {
                                                let chosen_type = alternative_types.choose(&mut thread_rng()).unwrap();
                                                new_type = Some(chosen_type.clone());
                                                match chosen_type.as_str() {
                                                    "i32" => new_value = Some(Expr::Lit(ExprLit {
                                                        attrs: Vec::new(),
                                                        lit: syn::Lit::Int(LitInt::new("0", Span::call_site())),
                                                    })),
                                                    "u32" => new_value = Some(Expr::Lit(ExprLit {
                                                        attrs: Vec::new(),
                                                        lit: syn::Lit::Int(LitInt::new("0u32", Span::call_site())),
                                                    })),
                                                    "i64" => new_value = Some(Expr::Lit(ExprLit {
                                                        attrs: Vec::new(),
                                                        lit: syn::Lit::Int(LitInt::new("0i64", Span::call_site())),
                                                    })),
                                                    "u64" => new_value = Some(Expr::Lit(ExprLit {
                                                        attrs: Vec::new(),
                                                        lit: syn::Lit::Int(LitInt::new("0u64", Span::call_site())),
                                                    })),
                                                    "f32" => new_value = Some(Expr::Lit(ExprLit {
                                                        attrs: Vec::new(),
                                                        lit: syn::Lit::Float(LitFloat::new("0.0f32", Span::call_site())),
                                                    })),
                                                    "f64" => new_value = Some(Expr::Lit(ExprLit {
                                                        attrs: Vec::new(),
                                                        lit: syn::Lit::Float(LitFloat::new("0.0", Span::call_site())),
                                                    })),
                                                    "char" => new_value = Some(Expr::Lit(ExprLit {
                                                        attrs: Vec::new(),
                                                        lit: syn::Lit::Char(LitChar::new('\'', Span::call_site())),
                                                    })),
                                                    "()" => new_value = Some(Expr::Lit(ExprLit {
                                                        attrs: Vec::new(),
                                                        lit: syn::Lit::Verbatim(proc_macro2::Literal::string("()")),
                                                    })),
                                                    _ => new_value = Some(Expr::Lit(ExprLit {
                                                        attrs: Vec::new(),
                                                        lit: syn::Lit::Verbatim(proc_macro2::Literal::string("()")),
                                                    })),
                                                }
                                            } else {
                                                new_value = Some(Expr::Lit(ExprLit {
                                                    attrs: Vec::new(),
                                                    lit: syn::Lit::Verbatim(proc_macro2::Literal::string("()")),
                                                }));
                                            }
                                        }
                                        syn::Lit::Char(_) => {
                                            let alternative_types = self.types_in_program.iter().filter(|t| *t != "char").collect::<Vec<_>>();
                                            if alternative_types.len() > 0 {
                                                let chosen_type = alternative_types.choose(&mut thread_rng()).unwrap();
                                                new_type = Some(chosen_type.clone());
                                                match chosen_type.as_str() {
                                                    "i32" => new_value = Some(Expr::Lit(ExprLit {
                                                        attrs: Vec::new(),
                                                        lit: syn::Lit::Int(LitInt::new("0", Span::call_site())),
                                                    })),
                                                    "u32" => new_value = Some(Expr::Lit(ExprLit {
                                                        attrs: Vec::new(),
                                                        lit: syn::Lit::Int(LitInt::new("0u32", Span::call_site())),
                                                    })),
                                                    "i64" => new_value = Some(Expr::Lit(ExprLit {
                                                        attrs: Vec::new(),
                                                       1lit: syn::Lit::Int(LitInt::new("0i64", Span::call_site())),
                                                    })),
                                                    "u64" => new_value = Some(Expr::Lit(ExprLit {
                                                        attrs: Vec::new(),
                                                        lit: syn::Lit::Int(LitInt::new("0u64", Span::call_site())),
                                                    })),
                                                    "f32" => new_value = Some(Expr::Lit(ExprLit {
                                                        attrs: Vec::new(),
                                                        lit: syn::Lit::Float(LitFloat::new("0.0f32", Span::call_site())),
                                                    })),
                                                    "f64" => new_value = Some(Expr::Lit(ExprLit {
                                                        attrs: Vec::new(),
                                                        lit: syn::Lit::Float(LitFloat::new("0.0", Span::call_site())),
                                                    })),
                                                    "bool" => new_value = Some(Expr::Lit(ExprLit {
                                                        attrs: Vec::new(),
                                                        lit: syn::Lit::Bool(LitBool {
                                                            value: false,
                                                            span: Span::call_site(),
                                                        }),
                                                    })),
                                                    "()" => new_value = Some(Expr::Lit(ExprLit {
                                                        attrs: Vec::new(),
                                                        lit: syn::Lit::Verbatim(proc_macro2::Literal::string("()")),
                                                    })),
                                                    _ => new_value = Some(Expr::Lit(ExprLit {
                                                        attrs: Vec::new(),
                                                        lit: syn::Lit::Verbatim(proc_macro2::Literal::string("()")),
                                                    })),
                                                }
                                            } else {
                                                new_value = Some(Expr::Lit(ExprLit {
                                                    attrs: Vec::new(),
                                                    lit: syn::Lit::Verbatim(proc_macro2::Literal::string("()")),
                                                }));
                                            }
                                        }
                                        _ => {
                                            new_value = Some(Expr::Lit(ExprLit {
                                                attrs: Vec::new(),
                                                lit: syn::Lit::Verbatim(proc_macro2::Literal::string("()")),
                                            }));
                                        }
                                    }
                                }
                                _ => {
                                    new_value = Some(Expr::Lit(ExprLit {
                                        attrs: Vec::new(),
                                        lit: syn::Lit::Verbatim(proc_macro2::Literal::string("()")),
                                    }));
                                }
                            }
                            if let Some(new_expr) = new_value {
                                expr_break.expr = Some(Box::new(new_expr));
                            }
                        }
                        syn::visit_mut::visit_expr_mut(self, expr);
                    }
                    _ => syn::visit_mut::visit_expr_mut(self, expr),
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets while loops within const contexts. It replaces the loop condition with `true` to force entry into the loop body. Additionally, it mutates any break expression that returns a value by changing its type to an incompatible type found in the seed program. This transformation aims to trigger type mismatches during const evaluation, potentially causing ICEs or compilation errors, similar to the reported bug."
    }
}