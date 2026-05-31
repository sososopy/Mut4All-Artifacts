//source file
#include "../include/operator_struct_field_mutation_111.h"

// ========================================================================================================
#define MUT1_OUTPUT 1

void MutatorFrontendAction_1::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("Fields")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getBeginLoc()))
        return;
      if (FD->isAnonymousStructOrUnion())
        return;
      auto type = FD->getType();
      if (type_candidates.empty())
        return;
      size_t index = getrandom::getRandomIndex(type_candidates.size() - 1);
      auto target_type = type_candidates[index];
      auto type_str =
          stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
      llvm::outs() << type_str << '\n';
      llvm::outs() << target_type->isStructureType() << '\n';
      if (type_str.find("/*mut1*/") != string::npos)
        return;
      if (target_type->isStructureType() || target_type->isUnionType())
        type_str = "/*mut1*/" + target_type->getAsTagDecl()->getNameAsString();
      else
        type_str = "/*mut1*/" + target_type->getCanonicalTypeInternal().getAsString();
      Rewrite.ReplaceText(FD->getBeginLoc(), 0, type_str);
    } else if (auto *TD = Result.Nodes.getNodeAs<clang::TagDecl>("Types")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getBeginLoc()))
        return;
      if (TD->isCompleteDefinition() == false)
        return;
      auto type = TD->getTypeForDecl();
      if (type->isStructureType() || type->isUnionType())
        type_candidates.push_back(type);
    } else if (auto *BT = Result.Nodes.getNodeAs<clang::BuiltinType>("BuiltIn")) {
      type_candidates.push_back(BT);
    }
}
  
void MutatorFrontendAction_1::MutatorASTConsumer_1::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto field_matcher = fieldDecl().bind("Fields");
    auto type_matcher = tagDecl().bind("Types");
    auto builtin_matcher = builtinType().bind("BuiltIn");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(field_matcher, &callback);
    matchFinder.addMatcher(type_matcher, &callback);
    matchFinder.addMatcher(builtin_matcher, &callback);
    matchFinder.matchAST(Context);
}