use syn::parse_quote;
use crate::mutator::Mutator;
use syn::Stmt;
use syn::Expr;

pub struct Replace_Var_Init_With_Ptr_Read_Const_425;

impl Mutator for Replace_Var_Init_With_Ptr_Read_Const_425 {
    fn name(&self) -> &str {
        "Replace_Var_Init_With_Ptr_Read_Const_425"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some(pat_type) = local.pat.as_type() {
                            let ty_clone = pat_type.ty.clone();
                            let new_expr: Expr = parse_quote! {
                                const { core::ptr::read::<#ty_clone>() }
                            };
                            *stmt = Stmt::Expr(new_expr, None);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces variable initializations with a const block invoking `core::ptr::read` using the variable's type. This transformation introduces an invalid const context with an intrinsic function call, potentially triggering ICEs related to const evaluation, type checking, or unsafe code handling."
    }
}