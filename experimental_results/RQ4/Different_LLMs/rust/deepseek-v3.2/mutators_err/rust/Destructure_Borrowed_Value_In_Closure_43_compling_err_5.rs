use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Local, Pat, PatType, Path as SynPath,
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

pub struct Destructure_Borrowed_Value_In_Closure_43;

impl Mutator for Destructure_Borrowed_Value_In_Closure_43 {
    fn name(&self) -> &str {
        "Destructure_Borrowed_Value_In_Closure_43"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct StructFinder {
            struct_names: HashSet<String>,
        }
        impl<'ast> Visit<'ast> for StructFinder {
            fn visit_item_struct(&mut self, i: &'ast ItemStruct) {
                self.struct_names.insert(i.ident.to_string());
            }
        }
        let mut finder = StructFinder {
            struct_names: HashSet::new(),
        };
        finder.visit_file(file);
        let struct_names = finder.struct_names;

        struct ClosureVisitor<'a> {
            struct_names: &'a HashSet<String>,
            mutations: Vec<(proc_macro2::Span, Box<Expr>, Box<Pat>)>,
        }
        impl<'ast, 'a> Visit<'ast> for ClosureVisitor<'a> {
            fn visit_expr_closure(&mut self, closure: &'ast ExprClosure) {
                let mut local_visitor = LocalVisitor {
                    struct_names: self.struct_names,
                    closure_span: closure.span(),
                    mutations: &mut self.mutations,
                };
                local_visitor.visit_expr_closure(closure);
            }
        }
        struct LocalVisitor<'a> {
            struct_names: &'a HashSet<String>,
            closure_span: proc_macro2::Span,
            mutations: &'a mut Vec<(proc_macro2::Span, Box<Expr>, Box<Pat>)>,
        }
        impl<'ast, 'a> Visit<'ast> for LocalVisitor<'a> {
            fn visit_local(&mut self, local: &'ast Local) {
                if let Some(init) = &local.init {
                    if let Expr::Reference(ref_expr) = &*init.expr {
                        if ref_expr.mutability.is_some() {
                            if let Pat::Ident(pat_ident) = &local.pat {
                                let var_name = pat_ident.ident.to_string();
                                let mut assignment_visitor = AssignmentVisitor {
                                    struct_names: self.struct_names,
                                    closure_span: self.closure_span,
                                    var_name,
                                    mutations: self.mutations,
                                };
                                assignment_visitor.visit_expr_closure(&ExprClosure {
                                    attrs: vec![],
                                    lifetimes: None,
                                    constness: None,
                                    asyncness: None,
                                    movability: None,
                                    capture: None,
                                    or1_token: token::Or::default(),
                                    inputs: Punctuated::new(),
                                    or2_token: token::Or::default(),
                                    output: ReturnType::Default,
                                    body: local.init.as_ref().unwrap().expr.clone(),
                                });
                            }
                        }
                    }
                }
            }
        }
        struct AssignmentVisitor<'a> {
            struct_names: &'a HashSet<String>,
            closure_span: proc_macro2::Span,
            var_name: String,
            mutations: &'a mut Vec<(proc_macro2::Span, Box<Expr>, Box<Pat>)>,
        }
        impl<'ast, 'a> Visit<'ast> for AssignmentVisitor<'a> {
            fn visit_expr_assign(&mut self, assign: &'ast ExprAssign) {
                if assign.span().source_text() == self.closure_span.source_text() {
                    if let Expr::Path(expr_path) = &*assign.right {
                        if expr_path.path.segments.len() == 1 {
                            let seg = &expr_path.path.segments[0];
                            if seg.ident.to_string() == self.var_name {
                                let mut type_visitor = TypeVisitor {
                                    struct_names: self.struct_names,
                                    found_struct: None,
                                };
                                type_visitor.visit_expr(&assign.right);
                                if let Some(struct_name) = type_visitor.found_struct {
                                    let pattern = create_struct_pattern(&struct_name);
                                    self.mutations.push((assign.span(), assign.right.clone(), pattern));
                                }
                            }
                        }
                    }
                }
            }
        }
        struct TypeVisitor<'a> {
            struct_names: &'a HashSet<String>,
            found_struct: Option<String>,
        }
        impl<'ast, 'a> Visit<'ast> for TypeVisitor<'a> {
            fn visit_type(&mut self, ty: &'ast Type) {
                if let Type::Reference(type_ref) = ty {
                    if let Type::Path(type_path) = &*type_ref.elem {
                        let last_seg = type_path.path.segments.last();
                        if let Some(seg) = last_seg {
                            let name = seg.ident.to_string();
                            if self.struct_names.contains(&name) {
                                self.found_struct = Some(name);
                            }
                        }
                    }
                }
            }
        }

        fn create_struct_pattern(struct_name: &str) -> Box<Pat> {
            let ident = Ident::new(struct_name, Span::call_site());
            let fields = Punctuated::<FieldPat, Comma>::new();
            Box::new(Pat::Struct(PatStruct {
                attrs: vec![],
                qself: None,
                path: SynPath::from(ident),
                brace_token: token::Brace::default(),
                fields,
                rest: None,
            }))
        }

        let mut visitor = ClosureVisitor {
            struct_names: &struct_names,
            mutations: Vec::new(),
        };
        visitor.visit_file(file);

        for (span, right_expr, pattern) in visitor.mutations {
            let mut replacer = MutReplacer {
                target_span: span,
                new_pattern: pattern,
                right_expr,
            };
            replacer.visit_file_mut(file);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets assignments inside closures where the right-hand side is a captured mutable reference to a struct. It changes the left-hand side to a struct pattern matching the struct type, using underscores for fields, without an explicit dereference. This mimics bug-triggering code like `V((_o,)) = v;` with `v` being `&mut V`, introducing a type mismatch and potential ICE similar to the reported bug."
    }
}

struct MutReplacer {
    target_span: proc_macro2::Span,
    new_pattern: Box<Pat>,
    right_expr: Box<Expr>,
}

impl VisitMut for MutReplacer {
    fn visit_expr_assign_mut(&mut self, assign: &mut ExprAssign) {
        if assign.span().source_text() == self.target_span.source_text() {
            assign.left = Box::new(Expr::Verbatim(quote! { #self.new_pattern }));
            assign.right = self.right_expr.clone();
        }
    }
}