use syn::{parse_quote, File, Item, Stmt, LocalInit, Expr, Type};
use proc_macro2::{Ident, Span};

pub struct Modify_Closure_Capture_And_Type_Context_14;

impl Mutator for Modify_Closure_Capture_And_Type_Context_14 {
    fn name(&self) -> &str {
        "Modify_Closure_Capture_And_Type_Context_14"
    }
    fn mutate(&self, file: &mut File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut closure_found = false;
                let mut new_var_stmt = None;
                let mut new_capture = None;
                for stmt in &mut item_fn.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Some(LocalInit { expr: init_expr, .. }) = &local.init {
                            if let Expr::Closure(closure) = &**init_expr {
                                closure_found = true;
                                let new_var_ident = Ident::new("y", Span::call_site());
                                let new_var_type: Type = parse_quote!(bool);
                                let new_var_expr: Expr = parse_quote!(true);
                                
                                new_var_stmt = Some(parse_quote!(let #new_var_ident: #new_var_type = #new_var_expr;));

                                new_capture = Some(parse_quote!(if #new_var_ident { println!("Boolean captured!"); }));
                                
                                if let Expr::Block(ref mut block) = *closure.body.clone() {
                                    block.block.stmts.push(parse_quote!(#new_capture));
                                }
                                break;
                            }
                        }
                    }
                }
                if let Some(stmt) = new_var_stmt {
                    item_fn.block.stmts.insert(0, stmt);
                }
                if closure_found {
                    break;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator identifies closures within function definitions, especially those used in complex contexts. It modifies the closure to capture an additional simple variable from the surrounding scope, adjusting the closure's type signature to reflect this new capture. This increases the likelihood of encountering issues related to closure captures and type inference."
    }
}