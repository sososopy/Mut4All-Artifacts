//source file
#include "../include/Complete_Incomplete_Struct_Definition_122.h"

// ========================================================================================================
#define MUT122_OUTPUT 1

void MutatorFrontendAction_122::Callback::run(const MatchFinder::MatchResult &Result) {
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
      llvm::outs() << content << '\n';
      if (content.find('}') == string::npos)
        return;
      auto fields = ST->fields();
      auto methods = ST->methods();
      int field_num = 0;
      for (auto field : fields) {
        field_num++;
      }
      int method_num = 0;
      for (auto method : methods) {
        method_num++;
      }
      if (field_num == 0 && method_num == 0)
        return;
      if (field_num == 0) {
        if (content.find('}') != string::npos)
          content.insert(content.rfind('}'), "/*mut122*/" + string("int mut122;"));
      } else if (method_num == 0) {
        if (content.find('}') != string::npos)
          content.insert(content.rfind('}'), "/*mut122*/" + string("void mut122(){}"));
      } else {
        int choice = getrandom::getRandomIndex(1);
        if (choice == 0) {
          if (content.find('}') != string::npos)
            content.insert(content.rfind('}'), "/*mut122*/" + string("int mut122;"));
        } else {
          if (content.find('}') != string::npos)
            content.insert(content.rfind('}'),
                           "/*mut122*/" + string("void mut122(){}"));
        }
      }

      Rewrite.ReplaceText(CharSourceRange::getTokenRange(ST->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_122::MutatorASTConsumer_122::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("Structs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}