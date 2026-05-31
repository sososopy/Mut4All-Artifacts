//source file
#include "../include/Indirect_Deleted_Ctor_In_Consteval_Class_69.h"

// ========================================================================================================
#define MUT69_OUTPUT 1

void MutatorFrontendAction_69::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ConstevalClass")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if ((!MT->isStruct() && !MT->isClass()) || MT->isLambda())
        return;
      if (!MT->isCompleteDefinition())
        return;
      if (MT->isAbstract())
        return;
      //Get the source code text of target node
      auto ctors = MT->ctors();
      bool has_consteval_ctor = false;
      bool has_deleted_ctor = false;
      for (auto ctor : ctors) {
        if (ctor->isConsteval())
          has_consteval_ctor = true;
        if (ctor->isDeleted())
          has_deleted_ctor = true;
      }
      if (!has_consteval_ctor || !has_deleted_ctor)
        return;
      auto methods = MT->methods();
      bool has_consteval_method = false;
      for (auto method : methods) {
        if (method->isConsteval())
          has_consteval_method = true;
      }
      if (!has_consteval_method)
        return;
      llvm::outs() << "Mut69: " << MT->getNameAsString() << " is a consteval class\n";
    }
    //Check whether the matched AST node is the target node
    else if (auto *MT = Result.Nodes.getNodeAs<clang::CXXConstructExpr>("DeletedCtor")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (!MT->getConstructor()->isDeleted())
        return;
      //Get the source code text of target node
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      text = "/*mut69*/" + text;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), text);
    }
    //Check whether the matched AST node is the target node
    else if (auto *MT = Result.Nodes.getNodeAs<clang::CXXConstructExpr>("ConstevalCtor")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (!MT->getConstructor()->isConsteval())
        return;
      //Get the source code text of target node
      auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                          MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      text = "/*mut69*/" + text;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), text);
    }
  }
  
void MutatorFrontendAction_69::MutatorASTConsumer_69::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto consteval_class_matcher = cxxRecordDecl().bind("ConstevalClass");
    auto deleted_ctor_matcher =
        cxxConstructExpr().bind("DeletedCtor");
    auto consteval_ctor_matcher =
        cxxConstructExpr().bind("ConstevalCtor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(consteval_class_matcher, &callback);
    matchFinder.addMatcher(deleted_ctor_matcher, &callback);
    matchFinder.addMatcher(consteval_ctor_matcher, &callback);
    matchFinder.matchAST(Context);
}