//source file
#include "../include/Insert_Designated_Initializer_In_Union_221.h"

// ========================================================================================================
#define MUT221_OUTPUT 1

void MutatorFrontendAction_221::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *UN = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Unions")) {
    if (!UN || !Result.Context->getSourceManager().isWrittenInMainFile(
                   UN->getLocation()))
      return;
    if (!UN->isUnion())
      return;
    if (!UN->isCompleteDefinition())
      return;
    auto fields = UN->fields();
    bool has_struct = false;
    bool has_class = false;
    bool has_non_numeric = false;
    for (auto field : fields) {
      if (field->getType()->isStructureType())
        has_struct = true;
      if (field->getType()->isClassType())
        has_class = true;
      if (!field->getType()->isIntegralOrEnumerationType())
        has_non_numeric = true;
    }
    if (has_non_numeric == false)
      return;
    if (has_struct == false && has_class == false)
      return;
    cur_union = UN;
  } else if (auto *NE = Result.Nodes.getNodeAs<clang::CXXNewExpr>("NewExpr")) {
    if (!NE || !Result.Context->getSourceManager().isWrittenInMainFile(
                   NE->getBeginLoc()))
      return;
    if (cur_union == nullptr)
      return;
    auto type = NE->getAllocatedType();
    if (type->isUnionType() == false)
      return;
    auto union_decl = type->getAsCXXRecordDecl();
    if (union_decl->getNameAsString() != cur_union->getNameAsString())
      return;
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), NE->getSourceRange());
    if (content.find('{') == string::npos)
      return;
    auto fields = cur_union->fields();
    std::vector<string> field_names;
    for (auto field : fields) {
      field_names.push_back(field->getNameAsString());
    }
    auto choice = getrandom::getRandomIndex(field_names.size() - 1);
    auto fieldname = field_names[choice];
    auto pos = content.find('{');
    content.insert(pos + 1, "." + fieldname + "=0,");
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(NE->getSourceRange()),
                        content);
  } else if (auto *DL =
                 Result.Nodes.getNodeAs<clang::VarDecl>("UnionDecl")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getBeginLoc()))
      return;
    if (cur_union == nullptr)
      return;
    auto type = DL->getType();
    if (type->isUnionType() == false)
      return;
    auto union_decl = type->getAsCXXRecordDecl();
    if (union_decl->getNameAsString() != cur_union->getNameAsString())
      return;
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
    if (content.find('{') == string::npos)
      return;
    auto fields = cur_union->fields();
    std::vector<string> field_names;
    for (auto field : fields) {
      field_names.push_back(field->getNameAsString());
    }
    auto choice = getrandom::getRandomIndex(field_names.size() - 1);
    auto fieldname = field_names[choice];
    auto pos = content.find('{');
    content.insert(pos + 1, "." + fieldname + "=0,");
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()),
                        content);
  }
}
  
void MutatorFrontendAction_221::MutatorASTConsumer_221::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto union_matcher = cxxRecordDecl().bind("Unions");
  auto new_matcher = cxxNewExpr().bind("NewExpr");
  auto decl_matcher = varDecl().bind("UnionDecl");
  Callback callback(TheRewriter);
  matchFinder.addMatcher(union_matcher, &callback);
  matchFinder.addMatcher(new_matcher, &callback);
  matchFinder.addMatcher(decl_matcher, &callback);
  matchFinder.matchAST(Context);
}