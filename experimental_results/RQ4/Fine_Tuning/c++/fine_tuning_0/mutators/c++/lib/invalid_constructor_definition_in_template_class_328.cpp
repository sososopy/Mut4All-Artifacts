//source file
#include "../include/Invalid_Constructor_Definition_In_Template_Class_328.h"

// ========================================================================================================
#define MUT328_OUTPUT 1

void MutatorFrontendAction_328::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Template")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto name = MT->getNameAsString();
      llvm::outs() << name << '\n';
      auto constructors = MT->ctors();
      //Perform mutation on the source code text by applying string replacement
      if (constructors.empty()) {
        string ins = name + "()";
        ins = "/*mut328*/" + ins;
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(MT->getEndLoc(), 0, ins);
      } else {
        auto constructor = *constructors.begin();
        auto def =
            stringutils::rangetoStr(*(Result.SourceManager), constructor->getSourceRange());
        llvm::outs() << def << '\n';
        Rewrite.ReplaceText(constructor->getSourceRange(), "");
        Rewrite.ReplaceText(MT->getEndLoc(), 0, def);
      }
    }
}
  
void MutatorFrontendAction_328::MutatorASTConsumer_328::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(isTemplateInstantiation()).bind("Template");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}