//source file
#include "../include/template_specialization_with_const_40.h"

// ========================================================================================================
#define MUT40_OUTPUT 1

void MutatorFrontendAction_40::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
      //Filter nodes in header files
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;

      for (auto *Spec : FT->specializations()) {
        if (auto *TTP = Spec->getTemplateSpecializationArgs()) {
          for (unsigned i = 0; i < TTP->size(); ++i) {
            if (TTP->get(i).getKind() == TemplateArgument::Type) {
              auto ArgType = TTP->get(i).getAsType();
              if (!ArgType.isConstQualified()) {
                //Get the source code text of target node
                std::string originalType = ArgType.getAsString();
                std::string modifiedType = "const " + originalType;
                //Perform mutation on the source code text by applying string replacement
                std::string originalDecl = Spec->getNameAsString();
                std::string mutatedDecl = originalDecl;
                size_t pos = mutatedDecl.find(originalType);
                if (pos != std::string::npos) {
                  mutatedDecl.replace(pos, originalType.length(), modifiedType);
                  //Replace the original AST node with the mutated one
                  Rewrite.ReplaceText(Spec->getSourceRange(), mutatedDecl);
                }
              }
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_40::MutatorASTConsumer_40::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl().bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}