//source file
#include "../include/Struct_Body_Mutation_291.h"

// ========================================================================================================
#define MUT291_OUTPUT 1

void MutatorFrontendAction_291::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("structDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto structname = stringutils::rangetoStr(*(Result.SourceManager),
                                                MT->getSourceRange());
      llvm::outs() << structname;
      //Perform mutation on the source code text by applying string replacement
      if (MT->isStruct() && MT->isCompleteDefinition() &&
          structname.find(';') != string::npos) {
        string body = "{ { int x = 0; } }";
        structname.insert(structname.find(';'), body);
        llvm::outs() << structname;
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(MT->getSourceRange()), structname);
      }
    }
}
  
void MutatorFrontendAction_291::MutatorASTConsumer_291::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl().bind("structDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}