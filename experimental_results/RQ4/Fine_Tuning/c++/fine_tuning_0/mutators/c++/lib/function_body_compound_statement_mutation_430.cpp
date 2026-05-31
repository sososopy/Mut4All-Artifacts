//source file
#include "../include/Function_Body_Compound_Statement_Mutation_430.h"

// ========================================================================================================
#define MUT430_OUTPUT 1

void MutatorFrontendAction_430::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->hasBody())
        return;
      if (FD->isVirtualAsWritten())
        return;
      auto body = FD->getBody();
      if (body->getStmtClass() != Stmt::CompoundStmtClass)
        return;
      auto body_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                  body->getSourceRange());
      std::string target_str = "";
      std::string target_view = "";
      std::string target_decl = "";
      auto decls = FD->decls();
      for (auto decl : decls) {
        if (decl->getKind() == Decl::Kind::Var) {
          auto vardecl = dyn_cast<VarDecl>(decl);
          auto type = vardecl->getType();
          if (type.getAsString() == "std::string") {
            target_str = vardecl->getNameAsString();
            target_view = "std::string_view mut_430_view = " + target_str +
                          ";\n/*mut430*/";
            break;
          }
        }
      }
      if (target_str == "") {
        target_decl =
            "std::string mut_430_str = \"mut430\";\n/*mut430*/std::string_view "
            "mut_430_view = mut_430_str;\n/*mut430*/";
      }
      body_content.insert(1, target_decl + target_view);
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()),
                          body_content);
    }
}
  
void MutatorFrontendAction_430::MutatorASTConsumer_430::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl().bind("functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}