//source file
#include "../include/Target_Structs_284.h"

// ========================================================================================================
#define MUT284_OUTPUT 1

void MutatorFrontendAction_284::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Structs")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->isStruct() == false)
      return;
    cur_structs.push_back(DL);
    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
    if (content.rfind('}') == string::npos)
      return;
    llvm::outs() << content << '\n';
    if (cur_structs.size() > 1) {
      auto last_struct = cur_structs[cur_structs.size() - 2];
      auto last_struct_name = last_struct->getNameAsString();
      llvm::outs() << last_struct_name << '\n';
      content.insert(content.rfind('}'), "\n\t" + last_struct_name + " mut284;");
    } else {
      content.insert(content.rfind('}'), "\n\tint mut284;");
    }
    content.insert(content.rfind('}'), "\n\tstruct { int mut284; };");
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
  }
}
  
void MutatorFrontendAction_284::MutatorASTConsumer_284::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Structs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}