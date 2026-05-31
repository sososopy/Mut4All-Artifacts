//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Constexpr_With_Extern_Constexpr_324
 */ 
class MutatorFrontendAction_324 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(324)

private:
    class MutatorASTConsumer_324 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_324(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl*> mutatedFunctions;
        std::vector<const clang::CXXRecordDecl*> classesWithConsteval;
        std::vector<const clang::VarDecl*> constinitVariables;
    };
};

//source file
#include "../include/Mutator_324.h"

// ========================================================================================================
#define MUT324_OUTPUT 1

void MutatorFrontendAction_324::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("ConstexprFunction")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Check if it's a1 constexpr function with a body
      if (!FD->isConstexpr() || !FD->hasBody())
        return;
      //Avoid functions already extern or defined elsewhere
      if (FD->getStorageClass() == SC_Extern)
        return;
      //Get the source code text of target node
      auto funcText = stringutils::rangetoStr(*(Result.SourceManager),
                                              FD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      //Find the function body start
      size_t bodyStart = funcText.find('{');
      if (bodyStart == std::string::npos)
        return;
      //Remove the body and add extern specifier
      std::string newDecl = funcText.substr(0, bodyStart);
      newDecl += ";";
      //Insert extern before constexpr
      size_t constexprPos = newDecl.find("constexpr");
      if (constexprPos != std::string::npos) {
        newDecl.insert(constexprPos, "extern ");
      }
      newDecl = "/*mut324*/" + newDecl;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), newDecl);
      mutatedFunctions.push_back(FD);
    }
    else if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassWithConsteval")) {
      //Filter nodes in header files
      if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RD->getLocation()))
        return;
      if (!RD->isCompleteDefinition())
        return;
      //Check for consteval constructors
      for (auto *Method : RD->methods()) {
        if (auto *Ctor = dyn_cast<CXXConstructorDecl>(Method)) {
          if (Ctor->isConsteval()) {
            classesWithConsteval.push_back(RD);
            break;
          }
        }
      }
    }
    else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("ConstinitVar")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (VD->hasAttr<clang::ConstInitAttr>()) {
        constinitVariables.push_back(VD);
      }
    }
    else if (auto *Ctor = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("ConstevalConstructor")) {
      //Filter nodes in header files
      if (!Ctor || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Ctor->getLocation()))
        return;
      if (!Ctor->isConsteval() || !Ctor->hasBody())
        return;
      //Get the source code text of target node
      auto ctorText = stringutils::rangetoStr(*(Result.SourceManager),
                                              Ctor->getSourceRange());
      //Find a mutated function to use
      if (mutatedFunctions.empty())
        return;
      size_t index = getrandom::getRandomIndex(mutatedFunctions.size() - 1);
      auto *MF = mutatedFunctions[index];
      std::string funcName = MF->getNameAsString();
      //Insert call to extern constexpr function in constructor body
      size_t bodyStart = c1torText.find('{');
      if (bodyStart == std::string::npos)
        return;
      std::string callExpr = funcName + "(0);";
      ctorText.insert(bodyStart + 1, "\n/*mut324*/" + callExpr + "\n");
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(Ctor->getSourceRange()), ctorText);
    }
    else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("ConstinitVarForMutation")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (!VD->hasAttr<clang::ConstInitAttr>() || !VD->hasInit())
        return;
      //Find a class with consteval constructor
      if (classesWithConsteval.empty())
        return;
      size_t index = getrandom::getRandomIndex(classesWithConsteval.size() - 1);
      auto *Class = classesWithConsteval[index];
      std::string className = Class->getNameAsString();
      //Get the source code text of target node
      auto varText = stringutils::rangetoStr(*(Result.SourceManager),
                                             VD->getSourceRange());
      //Replace initialization with constructor call
      size_t initStart = varText.find('=');
      if (initStart == std::string::npos)
        return;
      std::string newInit = " = " + className + "()";
      varText.replace(initStart, varText.length() - initStart, newInit);
      varText = "/*mut324*/" + varText;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), varText);
    }
}
  
void MutatorFrontendAction_324::MutatorASTConsumer_324::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher constexprMatcher = functionDecl(isConstexpr(), hasBody(anything())).bind("ConstexprFunction");
    DeclarationMatcher constevalMatcher = cxxRecordDecl(hasMethod(cxxConstructorDecl(isConsteval()))).bind("ClassWithConsteval");
    DeclarationMatcher constinitMatcher = varDecl(hasAttr(clang::attr::ConstInit)).bind("ConstinitVar");
    DeclarationMatcher constevalCtorMatcher = cxxConstructorDecl(isConsteval(), hasBody(anything())).bind("ConstevalConstructor");
    DeclarationMatcher constinitVarMutationMatcher = varDecl(hasAttr(clang::attr::ConstInit), hasInit()).bind("ConstinitVarForMutation");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(constexprMatcher, &callback);
    matchFinder.addMatcher(constevalMatcher, &callback);
    matchFinder.addMatcher(constinitMatcher, &callback);
    matchFinder.addMatcher(constevalCtorMatcher, &callback);
    matchFinder.addMatcher(constinitVarMutationMatcher, &callback);
    matchFinder.matchAST(Context);
}