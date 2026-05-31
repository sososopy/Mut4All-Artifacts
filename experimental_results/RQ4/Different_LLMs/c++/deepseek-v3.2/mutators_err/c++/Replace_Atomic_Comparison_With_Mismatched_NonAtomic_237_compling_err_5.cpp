//source file
#include "../include/Mutator_Replace_Atomic_Comparison_With_Mismatched_NonAtomic_237.h"

// ========================================================================================================
#define MUT237_OUTPUT 1

void MutatorFrontendAction_237::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("BinaryOp")) {
      //Filter nodes in header files
      if (!BO || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BO->getBeginLoc()))
        return;
      
      // Check if it's a comparison operator
      if (!BO->isComparisonOp()) return;
      
      // Get both operands
      Expr *LHS = BO->getLHS()->IgnoreImpCasts();
      Expr *RHS = BO->getRHS()->IgnoreImpCasts();
      
      // Determine if one is atomic and the other is non-atomic with mismatched signedness
      bool lhsAtomic = false, rhsAtomic = false;
      QualType lhsType, rhsType;
      std::string atomicVarName, nonAtomicVarName;
      QualType atomicUnderlyingType, nonAtomicType;
      
      // Check LHS
      if (LHS->getType()->isAtomicType()) {
        lhsAtomic = true;
        atomicVarName = getStmtAsString(LHS, *Result.SourceManager, *Result.Context);
        atomicUnderlyingType = LHS->getType().getNonReferenceType()->getAtomicUnqualifiedType();
        lhsType = atomicUnderlyingType;
      } else {
        lhsType = LHS->getType();
      }
      
      // Check RHS
      if (RHS->getType()->isAtomicType()) {
        rhsAtomic = true;
        atomicVarName = getStmtAsString(RHS, *Result.SourceManager, *Result.Context);
        atomicUnderlyingType = RHS->getType().getNonReferenceType()->getAtomicUnqualifiedType();
        rhsType = atomicUnderlyingType;
      } else {
        rhsType = RHS->getType();
      }
      
      // Must have exactly one atomic operand
      if (!(lhsAtomic ^ rhsAtomic)) return;
      
      // Determine which is the non-atomic operand
      QualType nonAtomicQualType = lhsAtomic ? rhsType : lhsType;
      std::string nonAtomicExpr = lhsAtomic ? getStmtAsString(RHS, *Result.SourceManager, *Result.Context) : getStmtAsString(LHS, *Result.SourceManager, *Result.Context);
      
      // Check if underlying types are integer types with mismatched signedness
      if (!atomicUnderlyingType->isIntegerType() || !nonAtomicQualType->isIntegerType()) return;
      
      bool atomicIsSigned = atomicUnderlyingType->isSignedIntegerType();
      bool nonAtomicIsSigned = nonAtomicQualType->isSignedIntegerType();
      
      // Must have mismatched signedness
      if (atomicIsSigned == nonAtomicIsSigned) return;
      
      // Get the atomic variable declaration to ensure it exists
      DeclRefExpr *atomicDeclRef = lhsAtomic ? dyn_cast<DeclRefExpr>(LHS) : dyn_cast<DeclRefExpr>(RHS);
      if (!atomicDeclRef) return;
      ValueDecl *atomicDecl = atomicDeclRef->getDecl();
      if (!atomicDecl) return;
      
      // Generate new non-atomic variable name
      std::string newVarName = atomicVarName + "_nonatomic";
      if (atomicVarMap.find(atomicVarName) != atomicVarMap.end()) {
        newVarName = atomicVarMap[atomicVarName];
      } else {
        atomicVarMap[atomicVarName] = newVarName;
      }
      
      //Get the source code text of target node
      std::string comparisonText = getStmtAsString(BO, *Result.SourceManager, *Result.Context);
      
      //Construct declaration for new non-atomic variable
      std::string atomicTypeStr = atomicUnderlyingType.getAsString();
      std::string declaration = atomicTypeStr + " " + newVarName + " = " + atomicVarName + ".load();\n";
      
      //Replace atomic operand with new non-atomic variable in comparison
      std::string mutatedComparison;
      if (lhsAtomic) {
        mutatedComparison = newVarName + " " + BO->getOpcodeStr().str() + " " + nonAtomicExpr;
      } else {
        mutatedComparison = nonAtomicExpr + " " + BO->getOpcodeStr().str() + " " + newVarName;
      }
      
      //Insert declaration before the comparison statement
      SourceLocation insertLoc = BO->getBeginLoc();
      Rewrite.InsertTextBefore(insertLoc, "/*mut237*/" + declaration);
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(BO->getSourceRange()), mutatedComparison);
    }
}
  
void MutatorFrontendAction_237::MutatorASTConsumer_237::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = binaryOperator(isComparisonOperator(),
                                              anyOf(hasLHS(hasType(isAtomic()))),
                                                    hasRHS(hasType(isAtomic()))).bind("BinaryOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}