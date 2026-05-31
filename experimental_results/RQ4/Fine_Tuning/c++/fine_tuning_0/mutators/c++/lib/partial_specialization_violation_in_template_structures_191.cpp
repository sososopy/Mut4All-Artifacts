//source file
#include "../include/Partial_Specialization_Violation_In_Template_Structures_191.h"

// ========================================================================================================
#define MUT191_OUTPUT 1

void MutatorFrontendAction_191::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *PS = Result.Nodes.getNodeAs<clang::ClassTemplatePartialSpecializationDecl>("PartialSpecializations")) {
      //Filter nodes in header files
      if (!PS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     PS->getLocation()))
        return;
      //Get the source code text of target node
      auto partial_specialization = stringutils::rangetoStr(*(Result.SourceManager), PS->getSourceRange());
      //Record the node information of the partial specialization
      cur_partial_specializations.push_back(PS);
      //Perform mutation on the source code text by applying string replacement
      partial_specialization = partial_specialization.substr(partial_specialization.find('>') + 1);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(PS->getSourceRange()), partial_specialization);
    }
}

void MutatorFrontendAction_191::MutatorASTConsumer_191::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    auto partial_specialization_matcher = classTemplatePartialSpecializationDecl().bind("PartialSpecializations");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(partial_specialization_matcher, &callback);
    matchFinder.matchAST(Context);
}