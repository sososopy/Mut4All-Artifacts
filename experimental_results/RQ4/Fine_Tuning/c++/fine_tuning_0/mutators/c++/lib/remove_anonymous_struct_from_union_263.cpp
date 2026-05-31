//source file
#include "../include/Remove_Anonymous_Struct_From_Union_263.h"

// ========================================================================================================
#define MUT263_OUTPUT 1

void MutatorFrontendAction_263::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *UN = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Unions")) {
      if (!UN || !Result.Context->getSourceManager().isWrittenInMainFile(
                     UN->getLocation()))
        return;

      if (!UN->isUnion())
        return;
      auto decls = UN->decls();
      for (auto decl : decls) {
        if (decl->getIdentifierNamespace() ==
            Decl::IdentifierNamespace::IDNS_Tag) {
          auto subdecl = dyn_cast<CXXRecordDecl>(decl);
          if (subdecl->isStruct()) {
            if (subdecl->getNameAsString() != "") {
              auto newdecl = stringutils::rangetoStr(
                  *(Result.SourceManager), subdecl->getSourceRange());
              llvm::outs() << newdecl << '\n';
              auto structpos = newdecl.find("struct");
              if (structpos != string::npos) {
                newdecl.erase(structpos + 6, subdecl->getNameAsString().size());
              }
              llvm::outs() << newdecl << '\n';
              Rewrite.ReplaceText(
                  CharSourceRange::getTokenRange(subdecl->getSourceRange()),
                  newdecl);
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_263::MutatorASTConsumer_263::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("Unions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}