//source file
#include "../include/Insert_Variadic_Friend_Function_In_Template_125.h"

// ========================================================================================================
#define MUT125_OUTPUT 1

void MutatorFrontendAction_125::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      llvm::outs() << declaration << '\n';
      if (declaration.find("typename...") == string::npos)
        return;
      auto pos = declaration.find_last_of('}');
      if (pos == string::npos)
        return;
      auto pos1 = declaration.find_last_of('>', pos);
      if (pos1 == string::npos)
        return;
      auto pos2 = declaration.find_last_of(' ', pos1);
      if (pos2 == string::npos)
        return;
      auto template_name = declaration.substr(pos2 + 1, pos1 - pos2);
      llvm::outs() << template_name << '\n';
      std::string friend_decl = "template<typename... Us> requires(... and c<Ts, Us>) friend bool operator==(A, A<Us...>) { return true; }";
      friend_decl = "/*mut125*/" + friend_decl;
      declaration.insert(pos, friend_decl);
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_125::MutatorASTConsumer_125::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}