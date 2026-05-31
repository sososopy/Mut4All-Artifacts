//source file
#include "../include/Replace_Member_Assignment_With_Static_228.h"

// ========================================================================================================
#define MUT228_OUTPUT 1

void MutatorFrontendAction_228::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::BinaryOperator>("Assignment")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Check if assignment is within a template class member function
      auto* FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("MemberFunc");
      if (!FD || !FD->isCXXClassMember())
        return;
      auto* RD = FD->getParent();
      if (!RD || !RD->isDependentContext())
        return;
      //Check if left-hand side is a member variable access
      auto* LHS = MT->getLHS();
      if (!LHS || !isa<clang::MemberExpr>(LHS))
        return;
      auto* ME = dyn_cast<clang::MemberExpr>(LHS);
      if (!ME || !ME->getMemberDecl() || !isa<clang::FieldDecl>(ME->getMemberDecl()))
        return;
      //Get the source code text of target node
      std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), MT->getSourceRange());
      //Select a visible class for qualification
      if (visibleClasses.empty())
        return;
      size_t idx = getrandom::getRandomIndex(visibleClasses.size() - 1);
      const clang::CXXRecordDecl* targetClass = visibleClasses[idx];
      std::string className = targetClass->getNameAsString();
      //Generate plausible static member name
      std::string staticMemberName = "_M_static_member_" + std::to_string(getrandom::getRandomIndex(1000));
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText = className + "::" + staticMemberName + " = " + stringutils::rangetoStr(*(Result.SourceManager), MT->getRHS()->getSourceRange());
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedText);
    }
    else if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("VisibleClass")) {
      //Record visible classes for later use
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if (CL->isCompleteDefinition())
        visibleClasses.push_back(CL);
    }
}
  
void MutatorFrontendAction_228::MutatorASTConsumer_228::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher assignmentMatcher = binaryOperator(isAssignmentOperator(),
                                                          hasLHS(memberExpr()),
                                                          hasParent(functionDecl().bind("MemberFunc"))).bind("Assignment");
    DeclarationMatcher classMatcher = cxxRecordDecl().bind("VisibleClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(assignmentMatcher, &callback);
    matchFinder.addMatcher(classMatcher, &callback);
    matchFinder.matchAST(Context);
}