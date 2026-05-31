//source file
#include "../include/Add_Template_Function_In_Attributed_Struct_312.h"

// ========================================================================================================
#define MUT312_OUTPUT 1

void MutatorFrontendAction_312::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ST = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Struct")) {
      if (!ST || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ST->getLocation()))
        return;
      if (!ST->isStruct())
        return;
      if (!ST->hasAttrs())
        return;
      if (ST->field_empty())
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), ST->getSourceRange());
      auto fields = ST->fields();
      auto field = fields.begin();
      auto fieldname = (*field)->getNameAsString();
      string func_def = "template <typename T> void mut_312() { (T)" +
                        fieldname + "; }\n";
      content.insert(content.rfind('}'), func_def);
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(ST->getSourceRange()),
                          content);
    }
}
  
void MutatorFrontendAction_312::MutatorASTConsumer_312::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("Struct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}