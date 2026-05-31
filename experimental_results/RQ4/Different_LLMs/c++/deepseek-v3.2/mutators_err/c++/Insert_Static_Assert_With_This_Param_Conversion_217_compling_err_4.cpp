//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Static_Assert_With_This_Param_Conversion_217
 */ 
class MutatorFrontendAction_217 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(217)

private:
    class MutatorASTConsumer_217 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_217(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl*> targetClasses;
        std::vector<std::string> conversionTypes;
        std::vector<std::string> binaryOps;
    };
};

//source file
#include "../include/Mutator_Insert_Static_Assert_With_This_Param_Conversion_217.h"

// ========================================================================================================
#define MUT217_OUTPUT 1

void MutatorFrontendAction_217::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("TargetClass")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (!MT->isCompleteDefinition())
        return;
      if (!MT->isStruct() && !MT->isClass())
        return;
      
      //Check if class has conversion operator and binary operator
      bool hasConversion = false;
      bool hasBinaryOp = false;
      std::string convType;
      std::string binOp;
      
      for (auto decl : MT->decls()) {
        if (auto *method = dyn_cast<CXXMethodDecl>(decl)) {
          if (method->getDeclName().getNameKind() == DeclarationName::CXXConversionFunctionName) {
            hasConversion = true;
            //Get conversion target type
            auto convFuncType = method->getReturnType();
            convType = convFuncType.getAsString();
          }
          if (method->isOverloadedOperator()) {
            auto opName = method->getNameAsString();
            if (opName.find("operator") != std::string::npos && 
                opName != "operator=" && opName != "operator++" && 
                opName != "operator--" && opName != "operator[]" &&
                opName != "operator->") {
              //Check if it's binary operator (has parameters)
              if (method->getNumParams() >= 1) {
                hasBinaryOp = true;
                binOp = opName;
              }
            }
          }
        }
      }
      
      if (hasConversion && hasBinaryOp) {
        targetClasses.push_back(MT);
        conversionTypes.push_back(convType);
        binaryOps.push_back(binOp);
      }
    }
    
    //Check whether the matched AST node is the target node
    else if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("ConversionOperator")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      
      //Find corresponding class
      auto *record = dyn_cast<CXXRecordDecl>(MT->getDeclContext());
      if (!record) return;
      
      bool found = false;
      size_t index = 0;
      for (size_t i = 0; i < targetClasses.size(); i++) {
        if (targetClasses[i] == record) {
          found = true;
          index = i;
          break;
        }
      }
      
      if (!found) return;
      
      //Get the source code text of target node
      auto sourceRange = MT->getSourceRange();
      if (sourceRange.isInvalid()) return;
      
      std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);
      
      //Check if conversion operator already has 'this' parameter
      bool hasThisParam = false;
      for (auto param : MT->parameters()) {
        if (param->getNameAsString() == "this") {
          hasThisParam = true;
          break;
        }
      }
      
      //Perform mutation on the1 source1 code text by applying string replacement
      if (!hasThisParam) {
        //Modify conversion operator to include 'this' parameter
        std::string mutatedText = originalText;
        
        //Find the parameter list start
        size_t paramStart = mutatedText.find('(');
        if (paramStart == std::string::npos) return;
        
        //Insert 'this' parameter
        std::string thisParam = "this const " + record->getNameAsString() + "& self";
        mutatedText.insert(paramStart + 1, thisParam + ", ");
        
        //Also add constexpr if not present
        if (!MT->isConstexpr()) {
          size_t operatorPos = mutatedText.find("operator");
          if (operatorPos != std::string::npos) {
            mutatedText.insert(operatorPos, "constexpr ");
          }
        }
        
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutatedText);
      }
    }
    
    //Check whether the matched AST node is the target node
    else if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionForStaticAssert")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      
      //Choose a random target class from collected ones
      if (targetClasses.empty()) return;
      size_t index = getrandom::getRandomIndex(targetClasses.size() - 1);
      auto *targetClass = targetClasses[index];
      std::string convType = conversionTypes[index];
      std::string binOp = binaryOps[index];
      
      //Get the source code text of target node
      auto sourceRange = MT->getSourceRange();
      if (sourceRange.isInvalid()) return;
      
      std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);
      
      //Perform mutation on the source code text by applying string replacement
      //Add static_assert using conversion in binary operator context
      std::string staticAssertText;
      
      //Create wrapper type if needed
      std::string wrapperName = "Wrap_" + targetClass->getNameAsString();
      
      //Check if wrapper already exists
      bool wrapperExists = false;
      for (auto decl : Result.Context->getTranslationUnitDecl()->decls()) {
        if (auto *record = dyn_cast<CXXRecordDecl>(decl)) {
          if (record->getNameAsString() == wrapperName) {
            wrapperExists = true;
            break;
          }
        }
      }
      
      if (!wrapperExists) {
        //Insert wrapper2 definition before function
        SourceLocation insertLoc = MT->getBeginLoc();
        std::string wrapperDef = "\ntemplate<typename T>\nstruct " + wrapperName + " {\n    T value;\n    constexpr " + wrapperName + "(T v) : value(v) {}\n};\n";
        Rewrite.InsertTextBefore(insertLoc, wrapperDef);
      }
      
      //Modify conversion operator to convert to wrapper
      //This requires finding and modifying the conversion operator separately
      //For simplicity, we'll assume it's been modified in previous step
      
      //Create static_assert text
      std::string className = targetClass->getNameAsString();
      staticAssertText = "\n    constexpr " + className + " s{};\n    static_assert((s " + binOp + " 10) == 52);\n";
      
      //Find function body end
      size_t bodyEnd = originalText.rfind('}');
      if (bodyEnd == std::string::npos) return;
      
      std::string mutatedText = originalText;
      mutatedText.insert(bodyEnd - 1, staticAssertText);
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutatedText);
    }
}
  
void MutatorFrontendAction_217::MutatorASTConsumer_217::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    //Match classes with conversion operators and binary operators
    DeclarationMatcher classMatcher = cxxRecordDecl(
        has(cxxMethodDecl(hasDeclName(DeclarationName::CXXConversionFunctionName))),
        has(cxxMethodDecl(isOverloadedOperator(), hasParameter(0, parmVarDecl()))),
        unless(isLambda())
    ).bind("TargetClass");
    
    //Match conversion operators
    DeclarationMatcher conversionMatcher = cxxMethodDecl(
        hasDeclName(DeclarationName::CXXConversionFunctionName),
        ofClass(cxxRecordDecl().bind("TargetClass"))
    ).bind("ConversionOperator");
    
    //Match functions where we can insert static_assert
    DeclarationMatcher functionMatcher = functionDecl(
        unless(cxxConstructorDecl()),
        unless(cxxDestructorDecl()),
        hasBody(compoundStmt())
    ).bind("FunctionForStaticAssert");
    
    Callback callback(TheRewriter);
    matchFinder.addMatcher(classMatcher, &callback);
    matchFinder.addMatcher(conversionMatcher, &callback);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.matchAST(Context);
}