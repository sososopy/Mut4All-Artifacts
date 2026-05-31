//source file
#include "../include/Struct_Incomplete_Declarator_List_360.h"

// ========================================================================================================
#define MUT360_OUTPUT 1

void MutatorFrontendAction_360::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Structs")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false)
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      auto fields = DL->fields();
      for (auto field : fields) {
        auto field_name = field->getNameAsString();
        auto field_type = field->getType().getAsString();
        llvm::outs() << field_name << ' ' << field_type << '\n';
        if (content.find(field_name) != string::npos)
          content.erase(content.find(field_type), field_type.size());
      }
      llvm::outs() << content << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()),
                          content);
    }
}
  
void MutatorFrontendAction_360::MutatorASTConsumer_360::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Structs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}