//source file
#include "../include/aligned_attribute_misalignment_607.h"

// ========================================================================================================
#define MUT607_OUTPUT 1

void MutatorFrontendAction_607::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("alignedVarDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;

      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 VD->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      size_t pos = declaration.find("__attribute__((aligned(");
      if (pos != std::string::npos) {
        size_t endPos = declaration.find(")))", pos);
        if (endPos != std::string::npos) {
          std::string mutatedAttr = "__attribute__((aligned((1ULL << 64))))";
          declaration.replace(pos, endPos - pos + 3, mutatedAttr);
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), declaration);
        }
      }
    }
}
  
void MutatorFrontendAction_607::MutatorASTConsumer_607::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasAttr(clang::attr::Aligned)).bind("alignedVarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}