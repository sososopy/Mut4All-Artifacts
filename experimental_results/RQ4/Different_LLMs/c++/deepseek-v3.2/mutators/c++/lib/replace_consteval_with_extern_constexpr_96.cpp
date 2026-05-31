//source file
#include "../include/Mutator_Replace_Consteval_With_Extern_Constexpr_96.h"

// ========================================================================================================
#define MUT96_OUTPUT 1

void MutatorFrontendAction_96::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("constinitVar")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      
      // Ensure it's a constinit variable
      if (!VD->hasAttr<clang::ConstInitAttr>())
        return;
      
      // Get the initializer expression
      const clang::Expr *Init = VD->getInit();
      if (!Init)
        return;
      
      // Check if initializer involves a consteval call (constructor or function)
      const clang::Expr *ConstevalExpr = nullptr;
      if (const auto *CE = llvm::dyn_cast<clang::CXXConstructExpr>(Init)) {
        if (CE->getConstructor()->isConsteval()) {
          ConstevalExpr = CE;
        }
      } else if (const auto *CE = llvm::dyn_cast<clang::CallExpr>(Init)) {
        if (const auto *FD = CE->getDirectCallee()) {
          if (FD->isConsteval()) {
            ConstevalExpr = CE;
          }
        }
      }
      
      if (!ConstevalExpr)
        return;
      
      // Add extern constexpr function declaration at global scope if not already added
      if (!externDeclAdded) {
        std::string externDecl = "extern constexpr void " + externFuncName + "();\n";
        SourceLocation InsertLoc = Result.Context->getSourceManager().getLocForStartOfFile(
            Result.Context->getSourceManager().getMainFileID());
        Rewrite.InsertText(InsertLoc, "/*mut96*/" + externDecl);
        externDeclAdded = true;
      }
      
      // For consteval constructor, we need to modify the constructor body
      if (const auto *CE = llvm::dyn_cast<clang::CXXConstructExpr>(ConstevalExpr)) {
        const clang::CXXConstructorDecl *Ctor = CE->getConstructor();
        if (Ctor->hasBody()) {
          const clang::Stmt *Body = Ctor->getBody();
          SourceLocation BodyStart = Body->getBeginLoc();
          // Insert extern function call at the beginning of constructor body
          std::string insertText = "/*mut96*/" + externFuncName + "(); ";
          Rewrite.InsertTextAfterToken(BodyStart, insertText);
        }
      }
      // For consteval function call, replace the call with extern function call
      else if (const auto *CE = llvm::dyn_cast<clang::CallExpr>(ConstevalExpr)) {
        std::string replacement = "/*mut96*/" + externFuncName + "()";
        Rewrite.ReplaceText(CE->getSourceRange(), replacement);
      }
    }
}
  
void MutatorFrontendAction_96::MutatorASTConsumer_96::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasAttr(attr::ConstInit)).bind("constinitVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}