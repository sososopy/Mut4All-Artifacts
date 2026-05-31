//source file
#include "../include/Enum_FwdDecl_Template_Instantiation_2.h"

// ========================================================================================================
#define MUT2_OUTPUT 1

void MutatorFrontendAction_2::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("EnumDecl")) {
      //Filter nodes in header files
      if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ED->getBeginLoc()))
        return;
      llvm::outs() << "EnumDecl: " << ED->getNameAsString() << "\n";
      //Record the node information of the forward-declared enum
      if (ED->isCompleteDefinition() == false) {
        llvm::outs() << "EnumDecl: " << ED->getNameAsString() << "\n";
        fwd_enum_decls.push_back(ED);
      }
    } else if (auto *TSI = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>(
                   "TemplateSpecialization")) {
      //Filter nodes in header files
      if (!TSI || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TSI->getBeginLoc()))
        return;
      if (fwd_enum_decls.empty())
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             TSI->getSourceRange());
      llvm::outs() << "TemplateSpecialization: " << content << "\n";
      //Perform mutation on the source code text by applying string replacement
      auto choice = getrandom::getRandomIndex(fwd_enum_decls.size() - 1);
      auto target_enum = fwd_enum_decls[choice];
      content = target_enum->getNameAsString();
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(TSI->getBeginLoc(), content.size(), content);
    }
}
  
void MutatorFrontendAction_2::MutatorASTConsumer_2::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto enum_matcher = enumDecl().bind("EnumDecl");
    auto template_specialization_matcher =
        classTemplateSpecializationDecl().bind("TemplateSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(enum_matcher, &callback);
    matchFinder.addMatcher(template_specialization_matcher, &callback);
    matchFinder.matchAST(Context);
}