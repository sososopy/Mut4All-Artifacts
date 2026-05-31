//source file
#include "../include/Mutator_427.h"

// ========================================================================================================
#define MUT427_OUTPUT 1

void MutatorFrontendAction_427::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::SwitchStmt>("SwitchStmt")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      
      const Expr* condition = MT->getCond();
      if (!condition)
        return;
      
      QualType condType = condition->getType();
      if (condType->isIntegralOrEnumerationType())
        return;
      
      const CXXRecordDecl* classDecl = nullptr;
      if (condType->isRecordType()) {
        classDecl = condType->getAsCXXRecordDecl();
      }
      
      if (!classDecl)
        return;
      
      // Check for conversion operator to enum type
      bool hasEnumConversion = false;
      QualType targetEnumType;
      for (auto method : classDecl->methods()) {
        if (method->getKind() == Decl::CXXConversion) {
          QualType convType = method->getReturnType();
          if (convType->isEnumeralType()) {
            hasEnumConversion = true;
            targetEnumType = convType;
            break;
          }
        }
      }
      
      if (!hasEnumConversion)
        //Get the source code text of target node
        return;
      
      auto condRange = CharSourceRange::getTokenRange(condition->getSourceRange());
      std::string condText = stringutils::rangetoStr(*Result.SourceManager, condition->getSourceRange());
      
      //Perform mutation on the source code text by applying string replacement
      std::string enumTypeName = targetEnumType.getAsString();
      std::string mutatedCond = "static_cast<" + enumTypeName + ">(" + condText + ")";
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(condRange, mutatedCond);
    }
    else if (auto *CD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassDecl")) {
      //Filter nodes in header files
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;
      
      // Check if this class has conversion to enum
      for (auto method : CD->methods()) {
        if (method->getKind() == Decl::CXXConversion) {
          QualType convType = method->getReturnType();
          if (convType->isEnumeralType()) {
            classesWithConversion.push_back(CD);
            break;
          }
        }
      }
    }
}
  
void MutatorFrontendAction_427::MutatorASTConsumer_427::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher switchMatcher = switchStmt().bind("SwitchStmt");
    DeclarationMatcher classMatcher = cxxRecordDecl().bind("ClassDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(switchMatcher, &callback);
    matchFinder.addMatcher(classMatcher, &callback);
    matchFinder.matchAST(Context);
}