//source file
#include "../include/insert_template_alias_in_template_struct_311.h"

// ========================================================================================================
#define MUT311_OUTPUT 1

void MutatorFrontendAction_311::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ST = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Structs")) {
      if (!ST || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ST->getLocation()))
        return;
      if (!ST->isStruct())
        return;
      if (ST->isCompleteDefinition() == false)
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), ST->getSourceRange());
      if (content.rfind('}') != string::npos) {
        content.insert(content.rfind('}'),
                       "\n\t/*mut311*/template<typename = int> using B = void;\n");
      }
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(ST->getSourceRange()),
                          content);
      cur_structs.push_back(ST);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("Decls")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      if (content.rfind('=') != string::npos) {
        content.replace(content.rfind('='), 1, "= /*mut311*/ T::template B<>");
      }
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()),
                          content);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("Insts")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (cur_structs.empty())
        return;
      auto target = cur_structs[getrandom::getRandomIndex(cur_structs.size() - 1)];
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      if (content.rfind('<') != string::npos) {
        content.replace(content.rfind('<'), 1, "< /*mut311*/" +
                                                   target->getNameAsString() +
                                                   ">");
      }
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()),
                          content);
    }
}
  
void MutatorFrontendAction_311::MutatorASTConsumer_311::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto struct_matcher = cxxRecordDecl().bind("Structs");
    auto decl_matcher = varDecl(hasType(cxxRecordDecl(hasName("T")))).bind("Decls");
    auto inst_matcher = varDecl(hasType(templateSpecializationType())).bind("Insts");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(struct_matcher, &callback);
    matchFinder.addMatcher(decl_matcher, &callback);
    matchFinder.addMatcher(inst_matcher, &callback);
    matchFinder.matchAST(Context);
}