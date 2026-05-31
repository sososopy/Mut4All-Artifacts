//source file
#include "../include/Mutator_Replace_Builtin_Assume_Aligned_Pointer_210.h"

// ========================================================================================================
#define MUT210_OUTPUT 1

void MutatorFrontendAction_210::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("BuiltinAssumeAlignedCall")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;
      
      // Check that this is indeed __builtin_assume_aligned
      const clang::FunctionDecl *FD = CE->getDirectCallee();
      if (!FD || FD->getNameAsString() != "__builtin_assume_aligned")
        return;
      
      // Ensure it has at least two arguments
      if (CE->getNumArgs() < 2)
        return;
      
      // First argument must be a pointer expression (type check)
      const clang::Expr *firstArg = CE->getArg(0);
      clang::QualType firstArgType = firstArg->getType();
      if (!firstArgType->isPointerType())
        return; // Already not a pointer, skip
        
      // Second argument must be an integer constant (alignment)
      const clang::Expr *secondArg = CE->getArg(1);
      if (!secondArg->getType()->isIntegerType())
        return;
      
      // Get the source code text of target node
      std::string originalCall = stringutils::rangetoStr(*(Result.SourceManager), CE->getSourceRange());
      
      // Collect non-pointer variables in current scope for potential replacement
      // This is done by matching variable declarations earlier in the translation unit
      // For simplicity, we assume availableNonPointerVars has been populated by other matchers
      
      // Choose replacement expression
      std::string replacementArg;
      bool needNewVar = false;
      
      // Random choice among available non-pointer replacements
      size_t totalOptions = availableNonPointerVars.size() + integerLiterals.size() + 
                         floatingLiterals.size() + characterLiterals.size() + boolLiterals.size();
      
      if (totalOptions == 0) {
        // No suitable replacement found, introduce a new integer variable
        needNewVar = true;
        replacementArg = "temp_mut210";
      } else {
        size_t choice = getrandom::getRandomIndex(totalOptions - 1);
        if (choice < availableNonPointerVars.size()) {
          const clang::VarDecl *var = availableNonPointerVars[choice];
          replacementArg = var->getNameAsString();
        } else if (choice < availableNonPointerVars.size() + integerLiterals.size()) {
          const clang::IntegerLiteral *lit = integerLiterals[choice - availableNonPointerVars.size()];
          replacementArg = stringutils::rangetoStr(*(Result.SourceManager), lit->getSourceRange());
        } else if (choice < availableNonPointerVars.size() + integerLiterals.size() + floatingLiterals.size()) {
          const clang::FloatingLiteral *lit = floatingLiterals[choice - availableNonPointerVars.size() - integerLiterals.size()];
          replacementArg = stringutils::rangetoStr(*(Result.SourceManager), lit->getSourceRange());
        } else if (choice < availableNonPointerVars.size() + integerLiterals.size() + floatingLiterals.size() + characterLiterals.size()) {
          const clang::CharacterLiteral *lit = characterLiterals[choice - availableNonPointerVars.size() - integerLiterals.size() - floatingLiterals.size()];
          replacementArg = stringutils::rangetoStr(*(Result.SourceManager), lit->getSourceRange());
        } else {
          const clang::CXXBoolLiteralExpr *lit = boolLiterals[choice - availableNonPointerVars.size() - integerLiterals.size() - floatingLiterals.size() - characterLiterals.size()];
          replacementArg = stringutils::rangetoStr(*(Result.SourceManager), lit->getSourceRange());
        }
      }
      
      // Perform mutation on the source code text by applying string replacement
      std::string mutatedCall;
      if (needNewVar) {
        // Insert variable declaration before the call
        SourceLocation insertLoc = CE->getBeginLoc();
        std::string varDecl = "int temp_mut210 = 0;\n";
        Rewrite.InsertTextBefore(insertLoc, varDecl);
        mutatedCall = "__builtin_assume_aligned(temp_mut210, ";
      } else {
        mutatedCall = "__builtin_assume_aligned(" + replacementArg + ", ";
      }
      
      // Keep the second argument (alignment)
      std::string secondArgText = stringutils::rangetoStr(*(Result.SourceManager), secondArg->getSourceRange());
      mutatedCall += secondArgText + ")";
      
      // Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()), mutatedCall);
    }
    
    // Also collect non-pointer variables and literals for use in replacements
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("NonPointerVar")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
        return;
      clang::QualType varType = VD->getType();
      if (!varType->isPointerType() && !varType->isReferenceType()) {
        availableNonPointerVars.push_back(VD);
      }
    }
    
    if (auto *IL = Result.Nodes.getNodeAs<clang::IntegerLiteral>("IntegerLiteral")) {
      if (!IL || !Result.Context->getSourceManager().isWrittenInMainFile(IL->getLocation()))
        return;
      integerLiterals.push_back(IL);
    }
    
    if (auto *FL = Result.Nodes.getNodeAs<clang::FloatingLiteral>("FloatingLiteral")) {
      if (!FL || !Result.Context->getSourceManager().isWrittenInMainFile(FL->getLocation()))
        return;
      floatingLiterals.push_back(FL);
    }
    
    if (auto *CL = Result.Nodes.getNodeAs<clang::CharacterLiteral>("CharacterLiteral")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(CL->getLocation()))
        return;
      characterLiterals.push_back(CL);
    }
    
    if (auto *BL = Result.Nodes.getNodeAs<clang::CXXBoolLiteralExpr>("BoolLiteral")) {
      if (!BL || !Result.Context->getSourceManager().isWrittenInMainFile(BL->getLocation()))
        return;
      boolLiterals.push_back(BL);
    }
}
  
void MutatorFrontendAction_210::MutatorASTConsumer_210::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher builtinMatcher = callExpr(callee(functionDecl(hasName("__builtin_assume_aligned")))).bind("BuiltinAssumeAlignedCall");
    
    // Matchers to collect replacement candidates
    DeclarationMatcher varMatcher = varDecl().bind("NonPointerVar");
    StatementMatcher intLiteralMatcher = integerLiteral().bind("IntegerLiteral");
    StatementMatcher floatLiteralMatcher = floatLiteral().bind("FloatingLiteral");
    StatementMatcher charLiteralMatcher = characterLiteral().bind("CharacterLiteral");
    StatementMatcher boolLiteralMatcher = cxxBoolLiteral().bind("BoolLiteral");
    
    Callback callback(TheRewriter);
    matchFinder.addMatcher(builtinMatcher, &callback);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.addMatcher(intLiteralMatcher, &callback);
    matchFinder.addMatcher(floatLiteralMatcher, &callback);
    matchFinder.addMatcher(charLiteralMatcher, &callback);
    matchFinder.addMatcher(boolLiteralMatcher, &callback);
    matchFinder.matchAST(Context);
}