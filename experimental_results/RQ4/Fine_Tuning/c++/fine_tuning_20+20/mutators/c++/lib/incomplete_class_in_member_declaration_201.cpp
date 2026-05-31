//source file
#include "../include/Incomplete_Class_in_Member_Declaration_201.h"

// ========================================================================================================
#define MUT201_OUTPUT 1

void MutatorFrontendAction_201::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if ((!MT->isStruct() && !MT->isClass()) || MT->isLambda())
        return;
      if (!MT->isCompleteDefinition())
        return;
      //Get the source code text of target node
      auto class_name = MT->getNameAsString();
      auto decls = MT->decls();
      for (auto decl : decls) {
        if (auto *FD = dyn_cast<FieldDecl>(decl)) {
          auto type = FD->getType();
          auto field_name = FD->getNameAsString();
          if (type->isBuiltinType() || type->isEnumeralType()) {
            if (field_name == class_name)
              field_name = "mut201_" + field_name;
            string field_decl = class_name + " " + field_name;
            llvm::outs() << field_decl << '\n';
            //Perform mutation on the source code text by applying string replacement
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(FD->getSourceRange(), field_decl);
            return;
          }
        }
      }
    }
}
  
void MutatorFrontendAction_201::MutatorASTConsumer_201::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}