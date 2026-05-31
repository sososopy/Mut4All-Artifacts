//source file
#include "../include/Replace_Dependent_TypeAlias_In_Template_187.h"

// ========================================================================================================
#define MUT187_OUTPUT 1

void MutatorFrontendAction_187::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("TypeAlias")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto TPL = MT->getDescribedAliasTemplate();
      if (TPL == nullptr)
        return;
      if (auto TPLS = dyn_cast<ClassTemplateSpecializationDecl>(TPL)) {
        auto type_aliases = TPLS->getTemplateArgs().asArray();
        if (type_aliases.size() < 2)
          return;
        auto original_type = MT->getUnderlyingType();
        int idx = -1;
        for (std::size_t i = 0; i < type_aliases.size(); ++i) {
          if (type_aliases[i].getAsType() == original_type) {
            idx = i;
            break;
          }
        }
        if (idx == -1)
          return;
        int new_idx = getrandom::getRandomIndex(type_aliases.size() - 1);
        if (new_idx >= idx)
          new_idx++;
        auto new_type = type_aliases[new_idx].getAsType();
        auto new_type_str = new_type.getAsString();
        //Perform mutation on the source code text by applying string replacement
        llvm::outs() << "/*mut187*/" << new_type_str << '\n';
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(MT->getBeginLoc(), original_type.getAsString().length(), new_type_str);
      }
    }
}
  
void MutatorFrontendAction_187::MutatorASTConsumer_187::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = typeAliasDecl().bind("TypeAlias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}