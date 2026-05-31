//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Union_Member_With_NonTrivial_Class_In_Template_8
 */ 
class MutatorFrontendAction_8 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(8)

private:
    class MutatorASTConsumer_8 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_8(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl*> nontrivialClasses;
        bool nontrivialClassAdded = false;
    };
};

//source file
#include "../include/Mutator_Replace_Union_Member_With_NonTrivial_Class_In_Template_8.h"

// ========================================================================================================
#define MUT8_OUTPUT 1

void MutatorFrontendAction_8::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("UnionField")) {
        //Filter nodes in header files
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
            return;
        
        // Check if field is inside a union within a template class
        auto *RD = FD->getParent();
        if (!RD || !RD->isUnion())
            return;
        
        auto *ParentRD = RD->getParent();
        if (!ParentRD || !isa<clang::TemplateDecl>(ParentRD))
            return;
        
        // Check if field type is trivial
        auto QT = FD->getType();
        if (!QT.isTrivialType(*Result.Context))
            return;
        
        // Find a non-trivial class type
        std::string newTypeName;
        if (!nontrivialClasses.empty()) {
            // Pick a random non-trivial class
            size_t idx = getrandom::getRandomIndex(nontrivialClasses.size() - 1);
            newTypeName = nontrivialClasses[idx]->getNameAsString();
        } else {
            // No non-trivial class found, create one
            if (!nontrivialClassAdded) {
                // Insert a simple non-trivial class at the beginning of the file
                SourceLocation StartLoc = Result.Context->getSourceManager().getLocForStartOfFile(
                    Result.Context->getSourceManager().getMainFileID());
                Rewrite.InsertText(StartLoc, "class nontrivial { nontrivial() {} }; ");
                nontrivialClassAdded = true;
            }
            newTypeName = "nontrivial";
        }
        
        //Get the source code text of target node
        auto fieldRange = FD->getSourceRange();
        std::string fieldText = stringutils::rangetoStr(*(Result.SourceManager), fieldRange);
        
        // Extract field name
        std::string fieldName = FD->getNameAsString();
        
        // Perform mutation: replace type with non-trivial class
        size_t typeEnd = fieldText.find(fieldName);
        if (typeEnd != std::string::npos) {
            std::string mutatedText = newTypeName + " " + fieldName;
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(fieldRange, mutatedText);
        }
    }
    else if (auto *CD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("NonTrivialClass")) {
        // Collect non-trivial classes for later use
        if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
            return;
        
        // Check if class is non-trivial (has user-provided constructor/destructor/virtual functions)
        bool isNonTrivial = false;
        if (CD->hasUserDeclaredConstructor() || CD->hasUserDeclaredDestructor()) {
            isNonTrivial = true;
        } else {
            // Check for virtual functions
            for (auto *Method : CD->methods()) {
                if (Method->isVirtual()) {
                    isNonTrivial = true;
                    break;
                }
            }
        }
        
        if (isNonTrivial) {
            nontrivialClasses.push_back(CD);
        }
    }
}
  
void MutatorFrontendAction_8::MutatorASTConsumer_8::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    // Matcher for union fields in template classes
    DeclarationMatcher unionFieldMatcher = fieldDecl(
        hasParent(recordDecl(isUnion())),
        hasParent(recordDecl(hasParent(decl(hasParent(decl(isa<clang::TemplateDecl>()))))))
    ).bind("UnionField");
    
    // Matcher for non-trivial classes
    DeclarationMatcher classMatcher = cxxRecordDecl(
        unless(isUnion()),
        unless(hasParent(decl(isa<clang::TemplateDecl>())))
    ).bind("NonTrivialClass");
    
    Callback callback(TheRewriter);
    matchFinder.addMatcher(unionFieldMatcher, &callback);
    matchFinder.addMatcher(classMatcher, &callback);
    matchFinder.matchAST(Context);
}