//source file
#include "../include/Insert_Static_Assert_In_Struct_Or_Union_238.h"

// ========================================================================================================
#define MUT238_OUTPUT 1

void MutatorFrontendAction_238::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Structs")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (MT->isStruct()) {
        if (declaration.find('{') != string::npos) {
          if (declaration.find('}') != string::npos) {
            auto members = MT->decls();
            if (std::distance(members.begin(), members.end()) == 0) {
              declaration.insert(declaration.rfind('}'),
                                 "/*mut238*/_Static_assert(!__builtin_types_compatible_p(int, int));");
            } else {
              auto member_index = getrandom::getRandomIndex(std::distance(members.begin(), members.end()) - 1);
              auto member = *(std::next(members.begin(), member_index));
              if (auto *VD = dyn_cast<VarDecl>(member)) {
                auto type = VD->getType().getAsString();
                declaration.insert(
                    declaration.rfind('}'),
                    "/*mut238*/_Static_assert(!__builtin_types_compatible_p(typeof(" +
                        VD->getNameAsString() + "), int));");
              } else if (auto *FD = dyn_cast<FieldDecl>(member)) {
                auto type = FD->getType().getAsString();
                declaration.insert(
                    declaration.rfind('}'),
                    "/*mut238*/_Static_assert(!__builtin_types_compatible_p(typeof(" +
                        FD->getNameAsString() + "), int));");
              } else if (auto *MD = dyn_cast<CXXMethodDecl>(member)) {
                auto type = MD->getReturnType().getAsString();
                declaration.insert(
                    declaration.rfind('}'),
                    "/*mut238*/_Static_assert(!__builtin_types_compatible_p(typeof(" +
                        MD->getNameAsString() + "), int));");
              } else {
                declaration.insert(declaration.rfind('}'),
                                   "/*mut238*/_Static_assert(!__builtin_types_compatible_p(int, int));");
              }
            }
          }
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_238::MutatorASTConsumer_238::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl().bind("Structs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}