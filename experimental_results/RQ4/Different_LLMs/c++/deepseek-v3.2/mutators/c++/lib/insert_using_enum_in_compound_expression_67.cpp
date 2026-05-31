//source file
#include "../include/Mutator_Insert_Using_Enum_In_Compound_Expression_67.h"

// ========================================================================================================
#define MUT67_OUTPUT 1

void MutatorFrontendAction_67::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("enumDecl")) {
      //Filter nodes in header files
      if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ED->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      if (ED->getDeclContext()->isRecord())
        targetEnums.push_back(ED);
    }
    else if (auto *BE = Result.Nodes.getNodeAs<clang::BinaryOperator>("binExpr")) {
      //Filter nodes in header files
      if (!BE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BE->getExprLoc()))
        return;
      if (!BE->isBitwiseOp() && !BE->isAdditiveOp())
        return;
      
      //Get the source code text of target node
      auto LHS = BE->getLHS()->IgnoreImpCasts();
      auto RHS = BE->getRHS()->IgnoreImpCasts();
      
      const clang::EnumConstantDecl* LHSEnum = nullptr;
      const clang::EnumConstantDecl* RHSEnum = nullptr;
      
      if (auto *DRE = clang::dyn_cast<clang::DeclRefExpr>(LHS))
        LHSEnum = clang::dyn_cast<clang::EnumConstantDecl>(DRE->getDecl());
      if (auto *DRE = clang::dyn_cast<clang::DeclRefExpr>(RHS))
        RHSEnum = clang::dyn_cast<clang::EnumConstantDecl>(DRE->getDecl());
      
      if (!LHSEnum && !RHSEnum)
        return;
      
      const clang::EnumDecl* targetEnum = nullptr;
      if (LHSEnum) targetEnum = clang::cast<clang::EnumDecl>(LHSEnum->getDeclContext());
      else if (RHSEnum) targetEnum = clang::cast<clang::EnumDecl>(RHSEnum->getDeclContext());
      
      if (!targetEnum || !targetEnum->getDeclContext()->isRecord())
        return;
      
      //Check if there's already a using enum in current scope
      bool hasUsingEnum = false;
      const clang::DeclContext* currentContext = targetEnum->getDeclContext();
      for (auto* decl : currentContext->decls()) {
        if (auto* UED = clang::dyn_cast<clang::UsingEnumDecl>(decl)) {
          if (UED->getEnumDecl() == targetEnum) {
            hasUsingEnum = true;
            break;
          }
        }
      }
      
      if (hasUsingEnum)
        return;
      
      //Find the parent record (class/struct) containing the enum
      const clang::RecordDecl* parentRecord = clang::cast<clang::RecordDecl>(targetEnum->getDeclContext());
      
      //Perform mutation on the source code text by applying string replacement
      std::string usingDecl = "using enum " + parentRecord->getNameAsString() + "::" + targetEnum->getNameAsString() + ";\n";
      
      //Insert before the expression
      clang::SourceLocation insertLoc = BE->getBeginLoc();
      clang::SourceManager& SM = Rewrite.getSourceMgr();
      
      //Find the start of the statement containing this expression
      clang::SourceLocation stmtStart = SM.getExpansionLoc(insertLoc);
      clang::PresumedLoc presumed = SM.getPresumedLoc(stmtStart);
      
      //Replace the original AST node with the mutated one
      Rewrite.InsertTextBefore(insertLoc, "/*mut67*/" + usingDecl);
    }
}
  
void MutatorFrontendAction_67::MutatorASTConsumer_67::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher enumMatcher = enumDecl().bind("enumDecl");
    StatementMatcher exprMatcher = binaryOperator(hasAnyOperatorName("|", "&", "+", "-")).bind("binExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(enumMatcher, &callback);
    matchFinder.addMatcher(exprMatcher, &callback);
    matchFinder.matchAST(Context);
}