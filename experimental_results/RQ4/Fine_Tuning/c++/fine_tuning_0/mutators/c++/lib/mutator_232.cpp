//source file
#include "../include/Mutator_232.h"

// ========================================================================================================
#define MUT232_OUTPUT 1

void MutatorFrontendAction_232::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      if ((!MT->isStruct() && !MT->isClass()) || MT->isLambda())
        return;
      if (!MT->isCompleteDefinition())
        return;
      auto dtor = MT->getDestructor();
      if (dtor == nullptr)
        return;
      auto fields = MT->fields();
      std::vector<FieldDecl *> private_field;
      std::vector<FieldDecl *> protected_field;
      std::vector<FieldDecl *> public_field;
      for (auto field : fields) {
        if (field->getAccess() == AccessSpecifier::AS_private)
          private_field.push_back(field);
        else if (field->getAccess() == AccessSpecifier::AS_protected)
          protected_field.push_back(field);
        else if (field->getAccess() == AccessSpecifier::AS_public)
          public_field.push_back(field);
      }
      //Perform mutation on the source code text by applying string replacement
      if (dtor->isImplicit()) {
        if (!private_field.empty()) {
          string fieldname = private_field.front()->getNameAsString();
          auto ins = "/*mut232*/~" + MT->getNameAsString() +
                     "(){ " + fieldname + " = 0; }";
          Rewrite.ReplaceText(MT->getEndLoc(), 0, ins);
        } else if (!protected_field.empty()) {
          string fieldname = protected_field.front()->getNameAsString();
          auto ins = "/*mut232*/~" + MT->getNameAsString() +
                     "(){ " + fieldname + " = 0; }";
          Rewrite.ReplaceText(MT->getEndLoc(), 0, ins);
        } else if (!public_field.empty()) {
          string fieldname = public_field.front()->getNameAsString();
          auto ins = "/*mut232*/~" + MT->getNameAsString() +
                     "(){ " + fieldname + " = 0; }";
          Rewrite.ReplaceText(MT->getEndLoc(), 0, ins);
        }
      } else if (dtor->isUserProvided()) {
        if (!private_field.empty()) {
          string fieldname = private_field.front()->getNameAsString();
          auto ins = "{ " + fieldname + " = 0; }";
          Rewrite.ReplaceText(dtor->getBody()->getSourceRange(), ins);
        } else if (!protected_field.empty()) {
          string fieldname = protected_field.front()->getNameAsString();
          auto ins = "{ " + fieldname + " = 0; }";
          Rewrite.ReplaceText(dtor->getBody()->getSourceRange(), ins);
        } else if (!public_field.empty()) {
          string fieldname = public_field.front()->getNameAsString();
          auto ins = "{ " + fieldname + " = 0; }";
          Rewrite.ReplaceText(dtor->getBody()->getSourceRange(), ins);
        }
      }
    }
}
  
void MutatorFrontendAction_232::MutatorASTConsumer_232::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}