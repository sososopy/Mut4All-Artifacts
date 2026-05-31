//source file
#include "../include/Mutator_297.h"

// ========================================================================================================
#define MUT297_OUTPUT 1

void MutatorFrontendAction_297::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("TemplateFunction")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Check if it's a template function with a body
      if (!FD->isTemplateInstantiation() && FD->getTemplatedKind() == FunctionDecl::TK_FunctionTemplate && FD->hasBody()) {
        //Get the source code text of target node
        Stmt *Body = FD->getBody();
        SourceRange BodyRange = Body->getSourceRange();
        std::string BodyText = stringutils::rangetoStr(*(Result.SourceManager), BodyRange);
        
        //Find a suitable insertion point: after the opening brace of the function body
        size_t InsertPos = BodyText.find("{") + 1;
        if (InsertPos == std::string::npos) return;
        
        //Check for existing variables in the function scope to capture
        std::string CaptureVarName = "captured_var";
        bool NeedVarDecl = true;
        //Look for local variable declarations in the function body
        if (auto *CompoundBody = dyn_cast<CompoundStmt>(Body)) {
          for (Stmt *S : CompoundBody->body()) {
            if (auto *DS = dyn_cast<DeclStmt>(S)) {
              for (Decl *D : DS->decls()) {
                if (auto *VD = dyn_cast<VarDecl>(D)) {
                  if (VD->getType()->isIntegerType()) {
                    CaptureVarName = VD->getNameAsString();
                    NeedVarDecl = false;
                    break;
                  }
                }
              }
            }
            if (!NeedVarDecl) break;
          }
        }
        
        //Prepare mutation insertion text
        std::string Insertion;
        if (NeedVarDecl) {
          Insertion = "\nint " + CaptureVarName + " = 0;\n";
        }
        Insertion += "auto lambda_" + CaptureVarName + " = [&" + CaptureVarName + "]() { }; lambda_" + CaptureVarName + "();";
        
        //Perform mutation on the source code text by applying string replacement
        BodyText.insert(InsertPos, Insertion);
        
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(BodyRange), BodyText);
      }
    }
}
  
void MutatorFrontendAction_297::MutatorASTConsumer_297::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isTemplateInstantiation()).bind("TemplateFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}