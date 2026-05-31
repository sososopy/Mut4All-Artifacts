//source file
#include "../include/Malform_Class_Member_Declaration_213.h"

// ========================================================================================================
#define MUT213_OUTPUT 1

void MutatorFrontendAction_213::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto curclass = stringutils::rangetoStr(*(Result.SourceManager),
                                              MT->getSourceRange());
      llvm::outs() << curclass << "\n";
      //Perform mutation on the source code text by applying string replacement
      auto decls = MT->decls();
      for (auto decl : decls) {
        if (decl->getIdentifierNamespace() ==
            Decl::IdentifierNamespace::IDNS_Ordinary) {
          llvm::outs() << "ordinary\n";
          auto vard = dyn_cast<VarDecl>(decl);
          if (vard == nullptr)
            continue;
          auto varname = vard->getNameAsString();
          auto pos = curclass.find(varname);
          if (pos == string::npos)
            continue;
          curclass.insert(pos, ";");
        } else if (decl->getIdentifierNamespace() ==
                   Decl::IdentifierNamespace::IDNS_OrdinaryFriend) {
          llvm::outs() << "ordinary friend\n";
          auto fund = dyn_cast<FunctionDecl>(decl);
          if (fund == nullptr)
            continue;
          auto params = fund->parameters();
          if (params.empty())
            continue;
          auto param0 = params[0];
          auto paramname = param0->getNameAsString();
          auto pos = curclass.find(paramname);
          if (pos == string::npos)
            continue;
          curclass.insert(pos, ";");
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), curclass);
    }
}
  
void MutatorFrontendAction_213::MutatorASTConsumer_213::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}