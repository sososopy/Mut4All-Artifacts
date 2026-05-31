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

pub struct Insert_Match_On_Opaque_Alias_In_Closure_483;

impl Mutator for Insert_Match_On_Opaque_Alias_In_Closure_483 {
    fn name(&self) -> &str {
        "Insert_Match_On_Opaque_Alias_In_Closure_483"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut opaque_aliases = Vec::new();
        let mut variable_decls = Vec::new();
        let mut closures = Vec::new();
        
        struct Collector {
            opaque_aliases: Vec<Ident>,
            variable_decls: Vec<(Ident, Ident)>,
            closures: Vec<(Ident, Ident)>,
        }
        
        impl<'ast> Visit<'ast> for Collector {
            fn visit_item(&mut self, node: &'ast Item) {
                if let Item::Type(item_type) = node {
                    if let Type::ImplTrait(_) = &*item_type.ty {
                        self.opaque_aliases.push(item_type.ident.clone());
                    }
                }
                syn::visit::visit_item(self, node);
            }
            
            fn visit_local(&mut self, node: &'ast Local) {
                let ty = node.init.as_ref().and_then(|init| {
                    if let Expr::Cast(expr_cast) = &*init.expr {
                        Some(&expr_cast.ty)
                    } else {
                        None
                    }
                });
                if let Some((pat, ty)) = extract_pat_and_type(&node.pat, ty) {
                    if let Type::Path(type_path) = ty {
                        if let Some(segment) = type_path.path.segments.last() {
                            let type_name = &segment.ident;
                            if self.opaque_aliases.contains(type_name) {
                                self.variable_decls.push((pat.clone(), type_name.clone()));
                            }
                        }
                    }
                }
                syn::visit::visit_local(self, node);
            }
            
            fn visit_expr_closure(&mut self, node: &'ast ExprClosure) {
                let mut visitor = ClosureCaptureVisitor {
                    captured_vars: Vec::new(),
                    opaque_aliases: &self.opaque_aliases,
                };
                visitor.visit_expr_closure(node);
                
                for (var_name, type_name) in visitor.captured_vars {
                    if self.opaque_aliases.contains(&type_name) {
                        self.closures.push((var_name, type_name));
                    }
                }
                syn::visit::visit_expr_closure(self, node);
            }
        }
        
        struct ClosureCaptureVisitor<'a> {
            captured_vars: Vec<(Ident, Ident)>,
            opaque_aliases: &'a Vec<Ident>,
        }
        
        impl<'a, 'ast> Visit<'ast> for ClosureCaptureVisitor<'a> {
            fn visit_expr_path(&mut self, node: &'ast ExprPath) {
                if let Some(segment) = node.path.segments.last() {
                    let var_name = &segment.ident;
                    for type_name in self.opaque_aliases {
                        self.captured_vars.push((var_name.clone(), type_name.clone()));
                    }
                }
                syn::visit::visit_expr_path(self, node);
            }
        }
        
        fn extract_pat_and_type<'a>(pat: &'a Pat, ty: Option<&'a Type>) -> Option<(Ident, &'a Type)> {
            match pat {
                Pat::Ident(pat_ident) => {
                    if let Some(ty) = ty {
                        Some((pat_ident.ident.clone(), ty))
                    } else {
                        None
                    }
                }
                _ => None,
            }
        }
        
        let mut collector = Collector {
            opaque_aliases: Vec::new(),
            variable_decls: Vec::new(),
            closures: Vec::new(),
        };
        collector.visit_file(file);
        
        opaque_aliases = collector.opaque_aliases;
        variable_decls = collector.variable_decls;
        closures = collector.closures;
        
        if opaque_aliases.is_empty() || variable_decls.is_empty() {
            return;
        }
        
        let mut rng = thread_rng();
        let target_var = variable_decls.choose(&mut rng);
        
        if let Some((var_name, type_name)) = target_var {
            let mut found_closure = false;
            
            struct ClosureModifier<'a> {
                var_name: &'a Ident,
                type_name: &'a Ident,
                modified: bool,
            }
            
            impl<'a> VisitMut for ClosureModifier<'a> {
                fn visit_expr_closure_mut(&mut self, node: &mut ExprClosure) {
                    if self.modified {
                        return;
                    }
                    
                    let mut visitor = ClosureCaptureChecker {
                        target_var: self.var_name,
                        found: false,
                    };
                    visitor.visit_expr_closure(&node);
                    
                    if visitor.found {
                        let match_expr = create_match_expr(self.var_name, self.type_name);
                        let stmt = Stmt::Expr(Expr::Match(match_expr), None);
                        
                        if let Expr::Block(expr_block) = &mut *node.body {
                            expr_block.block.stmts.insert(0, stmt);
                        } else {
                            let mut block: Block = parse_quote!({});
                            block.stmts.push(stmt);
                            node.body = Box::new(Expr::Block(ExprBlock {
                                attrs: Vec::new(),
                                label: None,
                                block,
                            }));
                        }
                        
                        self.modified = true;
                    }
                    
                    syn::visit_mut::visit_expr_closure_mut(self, node);
                }
            }
            
            struct ClosureCaptureChecker<'a> {
                target_var: &'a Ident,
                found: bool,
            }
            
            impl<'a, 'ast> Visit<'ast> for ClosureCaptureChecker<'a> {
                fn visit_expr_path(&mut self, node: &'ast ExprPath) {
                    if let Some(segment) = node.path.segments.last() {
                        if &segment.ident == self.target_var {
                            self.found = true;
                        }
                    }
                    syn::visit::visit_expr_path(self, node);
                }
            }
            
            let mut modifier = ClosureModifier {
                var_name,
                type_name,
                modified: false,
            };
            modifier.visit_file_mut(file);
            
            found_closure = modifier.modified;
            
            if !found_closure {
                let new_closure = create_new_closure(var_name, type_name);
                let closure_stmt: Stmt = parse_quote! {
                    let _closure = #new_closure;
                };
                
                struct StatementInserter {
                    target_var: Ident,
                    closure_stmt: Stmt,
                    inserted: bool,
                }
                
                impl VisitMut for StatementInserter {
                    fn visit_block_mut(&mut self, node: &mut Block) {
                        for (i, stmt) in node.stmts.iter().enumerate() {
                            if let Stmt::Local(local) = stmt {
                                if let Pat::Ident(pat_ident) = &local.pat {
                                    if pat_ident.ident == self.target_var {
                                        node.stmts.insert(i + 1, self.closure_stmt.clone());
                                        self.inserted = true;
                                        break;
                                    }
                                }
                            }
                        }
                        syn::visit_mut::visit_block_mut(self, node);
                    }
                }
                
                let mut inserter = StatementInserter {
                    target_var: var_name.clone(),
                    closure_stmt,
                    inserted: false,
                };
                inserter.visit_file_mut(file);
            }
        }
        
        fn create_match_expr(var_name: &Ident, type_name: &Ident) -> ExprMatch {
            let match_var: Expr = parse_quote!(#var_name);
            
            let pattern1 = create_pattern(type_name);
            let pattern2: Pat = parse_quote!(_);
            
            let arm1 = Arm {
                attrs: Vec::new(),
                pat: pattern1,
                guard: None,
                fat_arrow_token: Default::default(),
                body: Box::new(Expr::Block(ExprBlock {
                    attrs: Vec::new(),
                    label: None,
                    block: Block {
                        brace_token: Default::default(),
                        stmts: Vec::new(),
                    },
                })),
                comma: Some(Default::default()),
            };
            
            let arm2 = Arm {
                attrs: Vec::new(),
                pat: pattern2,
                guard: None,
                fat_arrow_token: Default::default(),
                body: Box::new(Expr::Block(ExprBlock {
                    attrs: Vec::new(),
                    label: None,
                    block: Block {
                        brace_token: Default::default(),
                        stmts: Vec::new(),
                    },
                })),
                comma: None,
            };
            
            ExprMatch {
                attrs: Vec::new(),
                match_token: Default::default(),
                expr: Box::new(match_var),
                brace_token: Default::default(),
                arms: vec![arm1, arm2].into_iter().collect(),
            }
        }
        
        fn create_pattern(type_name: &Ident) -> Pat {
            let mut rng = thread_rng();
            let pattern_type = rng.gen_range(0..3);
            
            match pattern_type {
                0 => parse_quote!(Some(_)),
                1 => parse_quote!((_, _)),
                2 => parse_quote!(Ok(_)),
                _ => parse_quote!(_),
            }
        }
        
        fn create_new_closure(var_name: &Ident, type_name: &Ident) -> ExprClosure {
            let match_expr = create_match_expr(var_name, type_name);
            
            ExprClosure {
                attrs: Vec::new(),
                lifetimes: None,
                constness: None,
                asyncness: None,
                movability: None,
                capture: None,
                or1_token: Default::default(),
                inputs: Punctuated::new(),
                or2_token: Default::default(),
                output: ReturnType::Default,
                body: Box::new(Expr::Match(match_expr)),
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies type aliases with impl Trait and variables of those types. It then inserts or modifies closures to include match expressions on these opaque type variables. This transformation combines opaque types, closure captures, and pattern matching to stress the compiler's discriminant analysis and MIR dataflow handling, particularly targeting bugs in the interaction between type alias impl Trait and control flow structures."
    }
}