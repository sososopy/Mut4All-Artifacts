//source file
#include "../include/incomplete_structure_for_constexpr_declaration_200.h"

// ========================================================================================================
#define MUT200_OUTPUT 1

void MutatorFrontendAction_200::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("ConstexprDecl")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isConstexpr() == false)
        return;
      auto type = DL->getType();
      if (type->isStructureType() == false)
        return;
      auto type_decl = type->getAsCXXRecordDecl();
      if (type_decl == nullptr)
        return;
      cur_classes.push_back(type_decl);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                   "StructDecl")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      bool flag = false;
      for (auto cl : cur_classes) {
        if (cl == DL) {
          flag = true;
          break;
        }
      }
      if (flag == false)
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      if (content.find(':') != string::npos) {
        content.erase(content.find(':'), content.size() - content.find(':'));
      } else {
        content.erase(content.find('{'), content.size() - content.find('{'));
      }
      content += "{}";
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()),
                          content);
    }
}
  
void MutatorFrontendAction_200::MutatorASTConsumer_200::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = varDecl().bind("ConstexprDecl");
    auto struct_matcher = cxxRecordDecl().bind("StructDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(struct_matcher, &callback);
    matchFinder.matchAST(Context);
}