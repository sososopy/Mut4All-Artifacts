//source file
#include "../include/Mutator_Replace_Decltype_With_Concrete_Type_500.h"

// ========================================================================================================
#define MUT500_OUTPUT 1

void MutatorFrontendAction_500::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::DecltypeType>("DecltypeNode")) {
      //Filter nodes in header files
      if (!MT) return;
      auto decltypeExpr = MT->getUnderlyingExpr();
      if (!decltypeExpr) return;
      if (!Result.Context->getSourceManager().isWrittenInMainFile(
                     decltypeExpr->getBeginLoc()))
        return;
      
      //Check if it's a member access expression like S::Value
      if (auto *ME = dyn_cast<MemberExpr>(decltypeExpr)) {
        if (ME->isArrow() || ME->getBase()->getType()->isPointerType()) return; // Skip pointer member access
        
        //Get the member's type
        QualType memberType = ME->getMemberDecl()->getType();
        
        //Find a suitable replacement type
        std::string replacementType;
        bool foundBuiltin = false;
        
        //Check if member type is built-in
        if (memberType->isBuiltinType()) {
            replacementType = memberType.getAsString();
            foundBuiltin = true;
        } else {
            //Look for user-defined types in availableTypes
            for (const Type* t : availableTypes) {
                if (t->getTypeClass() == memberType->getTypeClass()) {
                    replacementType = t->getAsCXXRecordDecl()->getNameAsString();
                    break;
                }
            }
            //If no suitable type found, use int if available
            if (replacementType.empty()) {
                for (const Type* t : availableTypes) {
                    if (t->isBuiltinType() && t->getAs<BuiltinType>()->getKind() == BuiltinType::Int) {
                        replacementType = "int";
                        break;
                    }
                }
            }
        }
        
        if (replacementType.empty()) return; // No valid replacement
        
        //Get the source range of the decltype expression
        SourceRange range = decltypeExpr->getSourceRange();
        if (!range.isValid()) return;
        
        //Perform mutation on the source code text by applying string replacement
        std::string replacement = replacementType;
        
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(range, replacement);
      }
    }
    //Also collect available types from the seed program
    else if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("RecordDecl")) {
      if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RD->getLocation()))
        return;
      if (RD->isCompleteDefinition()) {
        availableTypes.push_back(RD->getTypeForDecl());
      }
    }
    else if (auto *BT = Result.Nodes.getNodeAs<clang::BuiltinType>("BuiltinType")) {
      if (!BT) return;
      availableTypes.push_back(BT);
    }
}
  
void MutatorFrontendAction_500::MutatorASTConsumer_500::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    TypeMatcher decltypeMatcher = decltypeType().bind("DecltypeNode");
    DeclarationMatcher recordMatcher = cxxRecordDecl().bind("RecordDecl");
    TypeMatcher builtinMatcher = type(builtinType()).bind("BuiltinType");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(decltypeMatcher, &callback);
    matchFinder.addMatcher(recordMatcher, &callback);
    matchFinder.addMatcher(builtinMatcher, &callback);
    matchFinder.matchAST(Context);
}