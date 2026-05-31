//header file
#pragma once
#include "Mutator_base.h"

/**
 * Module_Interface_Unit_Conversion_442
 */ 
class MutatorFrontendAction_442 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(442)

private:
    class MutatorASTConsumer_442 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_442(Rewriter &R) : TheRewriter(R) {}
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
        std::string moduleName;
        bool moduleDeclFound;
        std::vector<const clang::FunctionDecl*> candidateFunctions;
        std::vector<const clang::VarDecl*> candidateVariables;
        std::vector<const clang::TypeDecl*> candidateTypes;
    };
};

//source file
#include "../include/Module_Interface_Unit_Conversion_442.h"

// ========================================================================================================
#define MUT442_OUTPUT 1

void MutatorFrontendAction_442::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MD = Result.Nodes.getNodeAs<clang::ModuleDecl>("ModuleDecl")) {
        //Filter nodes in header files
        if (!MD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MD->getLocation()))
            return;
        //Record that a module declaration already exists
        moduleDeclFound = true;
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
        //Filter nodes in header files
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
            return;
        //Record candidate function for export
        if (!FD->isMain() && FD->getAccess() == AS_public && !FD->hasBody()) {
            candidateFunctions.push_back(FD);
        }
    }
    else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
        //Filter nodes in header files
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
            return;
        //Record candidate variable for export
        if (VD->getAccess() == AS_public && VD->isFileVarDecl()) {
            candidateVariables.push_back(VD);
        }
    }
    else if (auto *TD = Result.Nodes.getNodeAs<clang::TypeDecl>("TypeDecl")) {
        //Filter nodes in header files
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
            return;
        //Record candidate type for module name
        if (TD->getAccess() == AS_public) {
            candidateTypes.push_back(TD);
        }
    }
    else if (auto *TU = Result.Nodes.getNodeAs<clang::TranslationUnitDecl>("TranslationUnit")) {
        //Filter nodes in header files
        if (!TU || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TU->getLocation()))
            return;
        //Get the source code text of target node
        //Perform mutation on the source code text by applying string replacement
        if (!moduleDeclFound) {
            //Determine module name
            if (!candidateTypes.empty()) {
                moduleName = candidateTypes[0]->getNameAsString();
            } else if (!candidateFunctions.empty()) {
                moduleName = candidateFunctions[0]->getNameAsString();
            } else if (!candidateVariables.empty()) {
                moduleName = candidateVariables[0]->getNameAsString();
            } else {
                moduleName = "mutated_module";
            }
            
            //Insert module declaration at beginning of file
            SourceLocation startLoc = TU->getBeginLoc();
            std::string moduleDecl = "export module " + moduleName + ";\n";
            Rewrite.InsertText(startLoc, moduleDecl);
            
            //Add export specifier to first candidate function if exists
            if (!candidateFunctions.empty()) {
                const FunctionDecl* FD = candidateFunctions[0];
                SourceLocation funcStart = FD->getBeginLoc();
                Rewrite.InsertText(funcStart, "export ");
            }
            
            //Add export specifier to first candidate variable if exists
            if (!candidateVariables.empty()) {
                const VarDecl* VD = candidateVariables[0];
                SourceLocation varStart = VD->getBeginLoc();
                Rewrite.InsertText(varStart, "export ");
            }
        }
    }
}
  
void MutatorFrontendAction_442::MutatorASTConsumer_442::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher moduleMatcher = moduleDecl().bind("ModuleDecl");
    DeclarationMatcher functionMatcher = functionDecl().bind("FunctionDecl");
    DeclarationMatcher varMatcher = varDecl().bind("VarDecl");
    DeclarationMatcher typeMatcher = typeDecl().bind("TypeDecl");
    DeclarationMatcher tuMatcher = translationUnitDecl().bind("TranslationUnit");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(moduleMat2cher, &callback);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.addMatcher(typeMatcher, &callback);
    matchFinder.addMatcher(tuMatcher, &callback);
    matchFinder.matchAST(Context);
}