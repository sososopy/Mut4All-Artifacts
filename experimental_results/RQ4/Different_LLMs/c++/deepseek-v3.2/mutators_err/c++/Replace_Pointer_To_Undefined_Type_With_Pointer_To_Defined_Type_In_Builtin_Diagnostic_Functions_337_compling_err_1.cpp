//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Pointer_To_Undefined_Type_With_Pointer_To_Defined_Type_In_Builtin_Diagnostic_Functions_337
 */ 
class MutatorFrontendAction_337 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(337)

private:
    class MutatorASTConsumer_337 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_337(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
        std::vector<const clang::CXXRecordDecl*> definedTypes;
    };
};

//source file
#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT337_OUTPUT 1

void MutatorFrontendAction_337::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CallExpr>("BuiltinCall")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      
      // Check if this is a __builtin_dump_struct call
      const FunctionDecl* FD = MT->getDirectCallee();
      if (!FD) return;
      std::string FuncName = FD->getNameAsString();
      if (FuncName != "__builtin_dump_struct") return;
      
      // Get the first argument (pointer argument)
      if (MT->getNumArgs() < 1) return;
      Expr* FirstArg = MT->getArg(0);
      QualType ArgType = FirstArg->getType();
      
      // Check if it's a pointer type
      if (!ArgType->isPointerType()) return;
      QualType PointeeType = ArgType->getPointeeType();
      
      // Check if the pointed-to type is incomplete
      const Type* PointeeBaseType = PointeeType.getTypePtr();
      if (PointeeBaseType->isIncompleteType()) {
        // Search for a complete defined type in our collected list
        if (definedTypes.empty()) return;
        
        // Randomly select a defined type
        size_t idx = getrandom::getRandomIndex(definedTypes.size() - 1);
        const CXXRecordDecl* SelectedType = definedTypes[idx];
        
        // Get source range of the call expression
        SourceRange CallRange = MT->getSourceRange();
        SourceManager& SM = *Result.SourceManager;
        
        // Generate new pointer variable name
        std::string NewVarName = "mut337_ptr_" + SelectedType->getNameAsString();
        
        // Check if there's an existing variable of this type we could use
        std::string Initializer = " = nullptr";
        
        // Create the declaration and initialization
        std::string NewDecl = SelectedType->getNameAsString() + "* " + NewVarName + Initializer + ";";
        
        // Get the location just before the call
        SourceLocation CallBegin = CallRange.getBegin();
        
        // Insert the declaration before the call
        Rewrite.InsertTextBefore(CallBegin, "/*mut337*/" + NewDecl + "\n");
        
        // Replace the original pointer argument with our new pointer variable
        Rewrite.ReplaceText(FirstArg->getSourceRange(), NewVarName);
      }
    }
    else if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DefinedType")) {
      // Collect defined types
      if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RD->getLocation()))
        return;
      
      if (RD->isCompleteDefinition() && !RD->isLambda()) {
        definedTypes.push_back(RD);
      }
    }
}
  
void MutatorFrontendAction_337::MutatorASTConsumer_337::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher builtinMatcher = callExpr(callee(functionDecl(hasName("__builtin_dump_struct")))).bind("BuiltinCall");
    DeclarationMatcher typeMatcher = cxxRecordDecl(isDefinition()).bind("DefinedType");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(builtinMatcher, &callback);
    matchFinder.addMatcher(typeMatcher, &callback);
    matchFinder.matchAST(Context);
}